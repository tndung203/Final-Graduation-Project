/*
 * bl_cmd_process.c
 *
 *  Created on: Apr 22, 2025
 *      Author: ADMIN
 */
#include "main.h"

unsigned char word_to_byte(unsigned int addr, int index, int lowerfirst) {
    unsigned char value = (addr >> (8 * (index - 1))) & 0x000000FF;
    return value;
}

unsigned int get_crc(unsigned char* buff, int length) {
    unsigned int crc = 0xFFFFFFFF;
    int i;

    for (i = 0; i < length; i++) {
        crc = crc ^ buff[i];
        for (int j = 0; j < 32; j++) {
            if (crc & 0x80000000) {
                crc = (crc << 1) ^ 0x04C11DB7;
            } else {
                crc = (crc << 1);
            }
        }
    }

    return crc;
}


/*This fuction sends ACK if CRC matches along with "length to follow"*/
void bootloader_send_ack(uint8_t command_code, uint8_t follow_len)
{
	char str[40];
	//here we send 2 byte.. first byte is ack and the second byte is len value
	uint8_t ack_buf[2];
	ack_buf[0] = BL_ACK;
	ack_buf[1] = follow_len;
	sprintf(str, "BL_ACK: 0x%X, len: %d\r\n", ack_buf[0], ack_buf[1]);
	HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

void bootloader_send_nack(void)
{
	char str[40];
	uint8_t nack = BL_NACK;
	sprintf(str, "BL_NACK: 0x%X\r\n", nack);
	HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}

//This verifies the CRC of the given buffer in pData
uint8_t bootloader_verify_crc (uint8_t *pData, uint32_t len, uint32_t crc_host)
 {
     uint32_t uwCRCValue=0xff;

     for (uint32_t i=0 ; i < len ; i++)
 	{
         uint32_t i_data = pData[i];
         uwCRCValue = HAL_CRC_Accumulate(&hcrc, &i_data, 1);
 	}

 	 /* Reset CRC Calculation Unit */
   __HAL_CRC_DR_RESET(&hcrc);

 	if( uwCRCValue == crc_host)
 	{
 		return VERIFY_CRC_SUCCESS;
 	}

 	return VERIFY_CRC_FAIL;
 }

uint8_t bl_get_version(){
	return (uint8_t)VERSION;
}

uint16_t bl_get_cid(){
	uint16_t cid;
	cid = (DBGMCU->IDCODE) & 0xFFF;
	return cid;
}

void bootloader_handle_getcid_cmd(uint8_t *bl_rx_buffer)
{
	uint8_t command_packet_len = *bl_rx_buffer + 1;
	uint32_t host_crc = *((uint32_t*) (bl_rx_buffer + command_packet_len - 4));
	if (!bootloader_verify_crc(bl_rx_buffer, 2, host_crc))
	{
		bootloader_send_ack(bl_rx_buffer[0], 1);
		uint16_t cid = bl_get_cid();
		char str[40];
		sprintf(str, "ID of Device: 0x%X\n", cid);
		HAL_UART_Transmit(&huart2, (uint8_t *) str, strlen(str), HAL_MAX_DELAY);
	}
	else{
		bootloader_send_nack();
	}
}

void bootloader_handle_getver_cmd(uint8_t* bl_rx_buffer)
{
	uint8_t command_package_len = bl_rx_buffer[0] + 1;

	uint32_t host_crc = *((uint32_t*) (bl_rx_buffer + command_package_len - 4));

	if (!bootloader_verify_crc(bl_rx_buffer, command_package_len - 4, host_crc))
	{
		bootloader_send_ack(bl_rx_buffer[0], 1);
		uint8_t bl_ver = bl_get_version();
		char str[40];
		sprintf(str, "Version: 0x%X\n", bl_ver);
		HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
	}

	else
	{
		bootloader_send_nack();
	}


}

void bootloader_go_to_addr(uint8_t *bl_rx_buffer)
{
    uint32_t go_address = 0;
    uint8_t command_package_len = bl_rx_buffer[0] + 1;
    uint32_t host_crc = *((uint32_t *)(bl_rx_buffer + command_package_len - 4));
    char *err = "CRC failed\n";
    char *err_go_addr = "Invalid go_address\n";

    // Verify CRC
    if (bootloader_verify_crc(bl_rx_buffer, command_package_len - 4, host_crc)) {
        bootloader_send_nack();
        xQueueSend(q_print, &err, portMAX_DELAY);
        return;
    }

    bootloader_send_ack(bl_rx_buffer[0], 1);
    go_address = *((uint32_t *)&bl_rx_buffer[2]);

    uint32_t msp_value = *(volatile uint32_t *)go_address;

    if (verify_address(go_address) == ADDR_INVALID)
    {
    	bootloader_send_nack();
    	xQueueSend(q_print, &err_go_addr, portMAX_DELAY);
    }

    uint32_t resethandler_address = *(volatile uint32_t *)(go_address + 4);

    // Disable interrupts and clear pending interrupts
    __disable_irq();
    NVIC->ICPR[0] = 0xFFFFFFFF; // Clear pending interrupts (adjust for your MCU)

    // Set VTOR to application’s vector table
    SCB->VTOR = go_address;

    // Configure MSP
    __set_MSP(msp_value);

    // Jump to application
    void (*app_reset_handler)(void) = (void *)resethandler_address;

    app_reset_handler();
}

//verify the address sent by the host .(check if address in range from start to end of add mem like sram1, sram2,...)
uint8_t verify_address(uint32_t go_address)
{
	//so, what are the valid addresses to which we can jump ?
	//can we jump to system memory ? yes
	//can we jump to sram1 memory ?  yes
	//can we jump to sram2 memory ? yes
	//can we jump to backup sram memory ? yes
	//can we jump to peripheral memory ? its possible , but dont allow. so no
	//can we jump to external memory ? yes.

//incomplete -poorly written .. optimize it
	if ( go_address >= SRAM1_BASE && go_address <= 0x2001BFFF)
	{
		return ADDR_VALID;
	}
	else if ( go_address >= SRAM2_BASE && go_address <= 0x2001FFFF)
	{
		return ADDR_VALID;
	}
	else if ( go_address >= FLASH_BASE && go_address <= FLASH_END)
	{
		return ADDR_VALID;
	}
//	else if ( go_address >= BKPSRAM_BASE && go_address <= BKPSRAM_END)
//	{
//		return ADDR_VALID;
//	}
	else
		return ADDR_INVALID;
}

/*This function writes the contents of pBuffer to  "mem_address" byte by byte */
//Note1 : Currently this function supports writing to Flash only .
//Note2 : This functions does not check whether "mem_address" is a valid address of the flash range.
uint8_t execute_mem_write(uint8_t *pBuffer, uint32_t mem_address, uint32_t len)
 {
     uint8_t status=HAL_OK;

     //We have to unlock flash module to get control of registers
     HAL_FLASH_Unlock();

     for(uint32_t i = 0 ; i <len ; i++)
     {
         //Here we program the flash byte by byte
         status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,mem_address+i,pBuffer[i] );
     }

     HAL_FLASH_Lock();

     return status;
 }

/*Helper function to handle BL_MEM_WRITE command */
void bootloader_handle_mem_write_cmd(uint8_t *pBuffer)
 {
 	uint8_t addr_valid = ADDR_VALID;
 	uint8_t write_status = 0x00;
 	uint8_t chksum =0, len=0;
 	len = pBuffer[0];
 	uint8_t payload_len = pBuffer[6];

 	uint32_t mem_address = *((uint32_t *) ( &pBuffer[2]) );

 	chksum = pBuffer[len];

     //Total length of the command packet
 	uint32_t command_packet_len = bl_rx_buffer[0]+1 ;

 	//extract the CRC32 sent by the Host
 	uint32_t host_crc = *((uint32_t * ) (bl_rx_buffer+command_packet_len - 4) ) ;


 	if (! bootloader_verify_crc(&bl_rx_buffer[0],command_packet_len-4,host_crc))
 	{

         bootloader_send_ack(pBuffer[0],1);

 		if( verify_address(mem_address) == ADDR_VALID )
 		{

             //glow the led to indicate bootloader is currently writing to memory

             //execute mem write
             write_status = execute_mem_write(&pBuffer[7],mem_address, payload_len);

 		}else
 		{
 			bootloader_send_nack();
 		}


 	}else
 	{
         bootloader_send_nack();
 	}

 }

void bootloader_read_data(uint8_t* bl_rx_buffer)
{
	switch(bl_rx_buffer[1])
	{
	case BL_GET_VER:
		bootloader_handle_getver_cmd(bl_rx_buffer);
		break;
	case BL_GET_CID:
		bootloader_handle_getcid_cmd(bl_rx_buffer);
		break;
	case BL_GO_TO_ADDR:
		bootloader_go_to_addr(bl_rx_buffer);
		break;
	case BL_MEM_WRITE:
		bootloader_handle_mem_write_cmd(bl_rx_buffer);
		break;
	}
}
