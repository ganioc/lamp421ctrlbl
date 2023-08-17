/*
 * d1_uart.c
 *
 *  Created on: 2023 Mar 13
 *      Author: ruffman
 */

#include "d1_uart.h"



usart_group_type usart_group_struct;


extern uint16_t aPacketSize;
extern uint8_t aPacketData[];


uint8_t data_take(void)
{
  uint8_t val;
  usart_interrupt_enable(USART1, USART_RDBF_INT, FALSE);
  usart_group_struct.count--;
  usart_interrupt_enable(USART1, USART_RDBF_INT, TRUE);

  val = usart_group_struct.buf[usart_group_struct.tail++];

  if(usart_group_struct.tail > (USART_REC_LEN-1))
    usart_group_struct.tail = 0;
  return val;
}

void data_send(uint8_t* buf, int len){
	int i;

	for(i = 0; i < len; i++){
		while(usart_flag_get(USART1, USART_TDBE_FLAG) == RESET);
		  usart_data_transmit(USART1, buf[i]);
	}
}

void init_d1_uart() {
	gpio_init_type gpio_init_struct;

	/* enable the usart2 and gpio clock */
	crm_periph_clock_enable(D1_UART_CRM_CLK, TRUE);
	crm_periph_clock_enable(D1_UART_TX_GPIO_CRM_CLK, TRUE);

	gpio_default_para_init(&gpio_init_struct);

	/* configure the usart2 tx pin */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_pins = D1_UART_TX_PIN;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(D1_UART_TX_GPIO, &gpio_init_struct);

	/* configure the usart2 rx pin */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_pins = D1_UART_RX_PIN;
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init(D1_UART_RX_GPIO, &gpio_init_struct);

	gpio_pin_mux_config(D1_UART_TX_GPIO,
			D1_UART_TX_PIN_SOURCE,
			D1_UART_TX_PIN_MUX_NUM);
	gpio_pin_mux_config(D1_UART_RX_GPIO,
			D1_UART_RX_PIN_SOURCE,
			D1_UART_RX_PIN_MUX_NUM);


	usart_init(D1_UART, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
	usart_parity_selection_config(D1_UART, USART_PARITY_NONE);

	usart_transmitter_enable(D1_UART, TRUE);
	usart_receiver_enable(D1_UART, TRUE);

	/* enable usart2 interrupt */
	usart_interrupt_enable(D1_UART, USART_RDBF_INT, TRUE);
	usart_interrupt_enable(D1_UART, USART_IDLE_INT, FALSE);

	usart_group_struct.head = 0;

	usart_enable(D1_UART, TRUE);

	/* config usart nvic interrupt */
	nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
	nvic_irq_enable(D1_UART_IRQn, 2, 0);

	usart_group_struct.head = 0;
	usart_group_struct.tail = 0;
}

/**
  * @brief  usart1 interrupt handler.
  * @param  none
  * @retval none
  */
void USART1_IRQHandler(void)
{
	uint16_t reval,i ;
	if (D1_UART->ctrl1_bit.rdbfien != RESET) {
		// time_ira_cnt = 0;  /* clear upgrade time out flag */
		if (usart_flag_get(USART1, USART_RDBF_FLAG) != RESET) {
			reval = usart_data_receive(USART1);
			usart_group_struct.buf[usart_group_struct.head++] = reval;

			if(usart_group_struct.head >= USART_REC_LEN){
				usart_group_struct.head = 0;
			}

			usart_interrupt_enable(D1_UART, USART_IDLE_INT, TRUE);

		}else
		if (usart_flag_get(D1_UART, USART_IDLEF_FLAG) != RESET) {

			if(usart_group_struct.tail != usart_group_struct.head){
				// copy to ymodem buffer,
//				for(int i = 0; i<usart_group_struct.head; i++){
//					aPacketData[i] = usart_group_struct.buf[i];
//					usart_group_struct.buf[i] = 0;
//				}
//				aPacketSize = usart_group_struct.head;
//
//				usart_group_struct.head = 0;
				i = 0;
				while(usart_group_struct.tail != usart_group_struct.head){
					aPacketData[i++] = usart_group_struct.buf[usart_group_struct.tail++];
					if(usart_group_struct.tail >= USART_REC_LEN){
						usart_group_struct.tail = 0;
					}
				}
				aPacketSize = i;
			}
			usart_interrupt_enable(D1_UART, USART_IDLE_INT, FALSE);

		}
	}
}



