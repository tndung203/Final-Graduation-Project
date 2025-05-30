/*
 * task_handler.c
 *
 *  Created on: Apr 16, 2025
 *      Author: ADMIN
 */

#include "main.h"

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
						"Control motor		----> 1\n"
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
				curr_state = sDC;
				xTaskNotify(DC_task, 0, eNoAction);
			}

		}

		else
		{
			xQueueSend(q_print, &msg_err, portMAX_DELAY);
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
	case sDC:
		xTaskNotify(DC_task, cmd, eSetValueWithOverwrite);
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

void handle_DC_task(void *params)
{
	const char *menu_msg = "Turn on motor	-->1\n"
							"Turn off motor	-->2\n"
							"Enter your choice here: ";
	uint32_t addr_buf;
	command_t *addr;
	while(1){
		xTaskNotifyWait(0,0,NULL, portMAX_DELAY);
		xQueueSend(q_print, &menu_msg, portMAX_DELAY);
		xTaskNotifyWait(0,0,&addr_buf,portMAX_DELAY);
		addr = ((command_t*)addr_buf);
		if (addr->buf[0] == '1')
			turn_on_dc();
		else if (addr->buf[0] == '2')
			turn_off_dc();
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

