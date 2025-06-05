/*
 * task_handler.c
 *
 *  Created on: Apr 16, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include <stdio.h>
#include "fatfs.h"

#define MyI2C_LCD I2C_LCD_1

volatile float vin = 0;
volatile uint32_t AD_RES = 0;

const char *msg_err = "====== Invalid command ======\n";
void handle_menu_task(void *params)
{
	const char* menu = "=========================\n"
						"|	Menu		|\n"
						"=========================\n"
						"LED effect		----> 0\n"
						"Bootloader command	----> 1\n"
						"Exit			----> 2\n"
						"Enter your choice here	: ";
	uint32_t addr_cmd;
	command_t *cmd;
	while(1)
	{
		xQueueSend(q_print, &menu, portMAX_DELAY);
		xTaskNotifyWait(0,0,&addr_cmd,portMAX_DELAY);
		cmd = ((command_t*) addr_cmd);
		if (cmd->len == 1)
 		{
			if (cmd->buf[0] == '0')
			{
				curr_state = sLed;
				xTaskNotify(led_task, 0, eNoAction);
			}

			else if (cmd->buf[0] == '1')
			{
				curr_state = sBlcmd;
				xTaskNotify(bl_command_task, 0, eNoAction);
			}
			else if (cmd->buf[0] == '2')
			{
				break;
			}
			else{
				xQueueSend(q_print, &msg_err, portMAX_DELAY);
				continue;
			}

		}

		else
		{
			xQueueSend(q_print, &msg_err, portMAX_DELAY);
			continue;
		}
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

	}
}

void handle_cmd_task(void *params)
{
	command_t cmd;
	while(1)
	{
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		process_command(&cmd);
	}
}

void process_command(command_t *cmd)
{
	int i = 0;
	uint8_t temp;
	do{
		xQueueReceive(q_data, &temp, portMAX_DELAY);
		cmd->buf[i++] = temp;
	}while(temp != '\n');
	cmd->len = i-1;
	cmd->buf[i-1] = '\0';
	switch(curr_state)
	{
	case sMenu:
		xTaskNotify(menu_task, cmd, eSetValueWithOverwrite);
		break;
	case sLed:
		xTaskNotify(led_task, cmd, eSetValueWithOverwrite);
		break;
	case sBlcmd:
		xTaskNotify(bl_command_task, cmd, eSetValueWithOverwrite);
		break;
	}
}

void handle_led_task(void *params)
{
	const char *led_effect = "Start blinking	----> e1\n"
						"Stop blinking	----> e2\n"
						"Enter your choice here: ";
	uint32_t addr_buf;
	command_t *addr;
	while(1)
	{
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
		xQueueSend(q_print, &led_effect, portMAX_DELAY);
		xTaskNotifyWait(0,0,&addr_buf,portMAX_DELAY);
		addr = ((command_t*)addr_buf);
		if (!strcmp((char*)addr->buf, "e1"))
			xTimerStart(xTimer, portMAX_DELAY);
		else if (!strcmp((char*)addr->buf, "e2"))
			xTimerStop(xTimer, portMAX_DELAY);
		else
			xQueueSend(q_print, &msg_err, portMAX_DELAY);
		curr_state = sMenu;
		xTaskNotify(menu_task, 0, eNoAction);
	}
}

void printmsg(void *params)
{
	while(1){
		char *str;
		xQueueReceive(q_print, &str, portMAX_DELAY);
		HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
	}
}

void handle_bl_cmd_task(void *params)
{
	const char *bl_cmds = "=================================\n"
							"|		Menu		|\n"
							"|	STM32F4 Bootloader	|\n"
							"=================================\n"
							"Which BL command do you want to send?\n"
							"BL_GET_VER		--> 1\n"
							"BL_GET_CID		--> 2\n"
							"BL_GO_TO_ADDR		--> 3\n"
							"BL_MEM_WRITE		--> 4\n"
							"Enter your choice: ";
	const char *go_to_addr = "Enter 4 bytes go address in hex: ";
	const char *mem_write = "Enter the memory write address here :";
	uint32_t addr_cmd;
	command_t * cmd;
	while(1)
	{
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		xQueueSend(q_print, &bl_cmds, portMAX_DELAY);
		xTaskNotifyWait(0, 0, &addr_cmd, portMAX_DELAY);
		cmd = (command_t*)addr_cmd;
		if (cmd->buf[0] == '1')
		{
			uint8_t len_to_follow = COMMAND_BL_GET_VER_LEN - 1;
			bl_rx_buffer[0] = len_to_follow;
			bl_rx_buffer[1] = BL_GET_VER;
			unsigned int crc32 = get_crc(&bl_rx_buffer[0], 2);
			crc32 = crc32 & 0xffffffff;
			bl_rx_buffer[2] = word_to_byte(crc32, 1, 1);
			bl_rx_buffer[3] = word_to_byte(crc32, 2, 1);
			bl_rx_buffer[4] = word_to_byte(crc32, 3, 1);
			bl_rx_buffer[5] = word_to_byte(crc32, 4, 1);
			bootloader_read_data(bl_rx_buffer);
		}
		else if (cmd->buf[0] == '2')
		{
			uint8_t len_to_follow = COMMAND_BL_GET_CID_LEN - 1;
			bl_rx_buffer[0] = len_to_follow;
			bl_rx_buffer[1] = BL_GET_CID;
			unsigned int crc32 = get_crc(&bl_rx_buffer[0], 2);
			crc32 = crc32 & 0xffffffff;
			bl_rx_buffer[2] = word_to_byte(crc32, 1, 1);
			bl_rx_buffer[3] = word_to_byte(crc32, 2, 1);
			bl_rx_buffer[4] = word_to_byte(crc32, 3, 1);
			bl_rx_buffer[5] = word_to_byte(crc32, 4, 1);
			bootloader_read_data(bl_rx_buffer);
		}
		else if (cmd->buf[0] == '3')
		{
			char go_address_str[11];
			xQueueSend(q_print, &go_to_addr, portMAX_DELAY);
			xTaskNotifyWait(0,0,&addr_cmd, portMAX_DELAY);
			cmd = (command_t*)addr_cmd;
			sprintf(go_address_str, "%s", cmd->buf);
			uint32_t hex_value;
			sscanf(go_address_str, "%x", &hex_value);
			uint8_t len_to_follow = COMMAND_BL_GO_TO_ADDR_LEN - 1;
			bl_rx_buffer[0] = len_to_follow;
			bl_rx_buffer[1] = BL_GO_TO_ADDR;
			bl_rx_buffer[2] = word_to_byte(hex_value, 1, 1);
			bl_rx_buffer[3] = word_to_byte(hex_value, 2, 1);
			bl_rx_buffer[4] = word_to_byte(hex_value, 3, 1);
			bl_rx_buffer[5] = word_to_byte(hex_value, 4, 1);
			unsigned int crc32 = get_crc(&bl_rx_buffer[0], COMMAND_BL_GO_TO_ADDR_LEN-4);
			crc32 = crc32 & 0xffffffff;
			bl_rx_buffer[6] = word_to_byte(crc32, 1, 1);
			bl_rx_buffer[7] = word_to_byte(crc32, 2, 1);
			bl_rx_buffer[8] = word_to_byte(crc32, 3, 1);
			bl_rx_buffer[9] = word_to_byte(crc32, 4, 1);
			bootloader_read_data(bl_rx_buffer);

		}
		else if (cmd->buf[0] == '4')
		{
			char mem_address_str[11];
			xQueueSend(q_print, &mem_write, portMAX_DELAY);
			xTaskNotifyWait(0,0,&addr_cmd, portMAX_DELAY);
			cmd = (command_t*)addr_cmd;
			sprintf(mem_address_str, "%s", cmd->buf);
			uint32_t mem_address;
			sscanf(mem_address_str, "%x", &mem_address);
			xTaskNotify(read_file, mem_address, eSetValueWithOverwrite);
		}
		else{
			xQueueSend(q_print, &msg_err, portMAX_DELAY);
		}
		curr_state = sMenu;
		xTaskNotify(menu_task, 0, eNoAction);
	}
}
