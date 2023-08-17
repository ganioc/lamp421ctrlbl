/*
 * d1_uart.h
 *
 *  Created on: 2023 Mar 13
 *      Author: ruffman
 */

#ifndef INCLUDE_D1_UART_H_
#define INCLUDE_D1_UART_H_

#include "at32f421_board.h"

/*******************D1 UART ***************************/
#define D1_UART                       USART1
#define D1_UART_CRM_CLK               CRM_USART1_PERIPH_CLOCK
#define D1_UART_TX_PIN                GPIO_PINS_9
#define D1_UART_RX_PIN                GPIO_PINS_10
#define D1_UART_TX_GPIO               GPIOA
#define D1_UART_RX_GPIO               GPIOA
#define D1_UART_TX_GPIO_CRM_CLK       CRM_GPIOA_PERIPH_CLOCK
#define D1_UART_IRQn                  USART1_IRQn

#define D1_UART_TX_PIN_SOURCE         GPIO_PINS_SOURCE9
#define D1_UART_RX_PIN_SOURCE         GPIO_PINS_SOURCE10
#define D1_UART_TX_PIN_MUX_NUM        GPIO_MUX_1
#define D1_UART_RX_PIN_MUX_NUM        GPIO_MUX_1

#define USART_REC_LEN                 (2048 + 24)

// #define PRINT_LOG(...)                cust_printf(__VA_ARGS__)
#define PRINT_LOG(...)

typedef struct
{
  uint16_t buf[USART_REC_LEN];
  uint16_t head;
  uint16_t tail;
  uint16_t count;
} usart_group_type;

void init_d1_uart();
uint8_t data_take(void);
void data_send(uint8_t* buf, int len);

#endif /* INCLUDE_D1_UART_H_ */
