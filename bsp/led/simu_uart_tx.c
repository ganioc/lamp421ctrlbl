/*
 * simu_uart_tx.c
 *
 *  Created on: 2023 Mar 13
 *      Author: ruffman
 */


#include "at32f421_board.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define UART_TX_ON()  led_eth_on()
#define UART_TX_OFF() led_eth_off()

#define SIMU_UART_BUFFER_LEN    256

enum UART_TX_STATE{
	STATE_DUMMY = 0,
	STATE_START,
	STATE_DATA,
	STATE_STOP
};

enum UART_TX_STATE state = STATE_DUMMY;

static uint8_t buffer[SIMU_UART_BUFFER_LEN], current_byte = 0 , current_bit = 0;
static uint16_t  buffer_head = 0;
static uint16_t  buffer_tail = 0;

void cust_printf(char* format, ...){
	char temp[256];

	va_list args_list;

	va_start(args_list, format);

	vsprintf(temp, format, args_list);

	//printf(temp);

	for(int i = 0; i< strlen(temp); i++){
		buffer[buffer_head++] = temp[i];
		if(buffer_head >= SIMU_UART_BUFFER_LEN){
			buffer_head = 0;
		}
	}

	va_end(args_list);
}


void simu_uart_tx(){
	switch(state){
	case STATE_DUMMY:
		if(buffer_head != buffer_tail){
			current_byte =  buffer[buffer_tail++];
			if(buffer_tail >= SIMU_UART_BUFFER_LEN){
				buffer_tail = 0;
			}
			state = STATE_START;
			UART_TX_ON();
		}
		break;
	case STATE_START:
		led_eth_off();
		state = STATE_DATA;
		current_bit = 0;
		break;
	case STATE_DATA:
		if(((current_byte >> current_bit++ ) & 0x01) == 0){
			UART_TX_OFF();
		}else{
			UART_TX_ON();
		}
		if(current_bit >= 8){
			state = STATE_STOP;
		}

		break;

	case STATE_STOP:
		state = STATE_DUMMY;
		UART_TX_ON();
		break;
	default:

		break;
	}

}

