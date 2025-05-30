/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "I2C_LCD.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct {
	uint8_t buf[10];
	uint8_t len;
}command_t;

typedef enum{
	sMenu,
	sLed,
	sBlcmd
}state;

typedef struct
{
	float Temperature;
	float Humidity;
}DHT_DataTypedef;

extern uint8_t bl_rx_buffer[255];

extern ADC_HandleTypeDef hadc1;

extern CRC_HandleTypeDef hcrc;

extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim4;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern TaskHandle_t cmd_task;
extern TaskHandle_t measure_task;
extern TaskHandle_t msg_task;
extern TaskHandle_t menu_task;
extern TaskHandle_t led_task;
extern TaskHandle_t bl_command_task;
extern QueueHandle_t q_print;
extern QueueHandle_t q_data;
extern TaskHandle_t read_file;

extern TimerHandle_t xTimer;

extern state curr_state;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void handle_measure_task(void *params);
void handle_menu_task(void *params);
void handle_cmd_task(void *params);
void handle_led_task(void *params);
void handle_bl_cmd_task(void *params);
void vTimerCallback (TimerHandle_t xTimer);
void bootloader_read_data(uint8_t* bl_rx_buffer);
void bootloader_send_ack(uint8_t command_code, uint8_t follow_len);
void bootloader_send_nack(void);
uint8_t bootloader_verify_crc(uint8_t *pData, uint32_t len, uint32_t crc_host);
uint8_t bl_get_version();
void bootloader_handle_getver_cmd(uint8_t* bl_rx_buffer);
void delay_us(volatile uint16_t us);
void delay_ms(volatile uint16_t ms);
void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void DHT_Start(void);
uint8_t DHT_Check_Response(void);
uint8_t DHT_Read(void);
void DHT_GetData(DHT_DataTypedef *DHT_Data);
void printmsg(void *params);
void process_command(command_t *cmd);
unsigned char word_to_byte(unsigned int addr, int index, int lowerfirst);
unsigned int get_crc(unsigned char* buff, int length);
void package_get_rs(uint8_t command_len, uint8_t command_code, uint8_t *bl_rx_buffer);
uint16_t bl_get_cid();
void bootloader_handle_getcid_cmd(uint8_t *bl_rx_buffer);
void bootloader_go_to_addr(uint8_t * bl_rx_buffer);
uint8_t verify_address(uint32_t go_address);
uint8_t execute_mem_write(uint8_t *pBuffer, uint32_t mem_address, uint32_t len);
void bootloader_handle_mem_write_cmd(uint8_t *bl_rx_buffer);
void SDIO_SDCard(void *params);



/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
#define VERSION 0x10
#define CID

#define BL_ACK 	0xA5
#define BL_NACK 0x7F

#define ADDR_VALID 0x00
#define ADDR_INVALID 0x01

#define VERIFY_CRC_SUCCESS 0
#define VERIFY_CRC_FAIL	   1

#define BL_GET_VER 		0x51
#define BL_GET_HELP		0x52
#define BL_GET_CID		0x53
#define BL_GET_RDP_STATUS	0x54
#define BL_GO_TO_ADDR	0x55
#define BL_MEM_WRITE	0x57

//len details of the command
#define COMMAND_BL_GET_VER_LEN		6
#define COMMAND_BL_GET_CID_LEN		6
#define COMMAND_BL_GO_TO_ADDR_LEN	10
#define COMMAND_BL_MEM_WRITE_LEN	11

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
