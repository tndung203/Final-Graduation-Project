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
	sDC
}state;

typedef struct
{
	float Temperature;
	float Humidity;
}DHT_DataTypedef;

extern uint8_t bl_rx_buffer[200];

extern ADC_HandleTypeDef hadc1;

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
extern TaskHandle_t DC_task;

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
void vTimerCallback (TimerHandle_t xTimer);
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
void handle_DC_task(void *params);
void turn_on_dc(void);
void turn_off_dc(void);




/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
