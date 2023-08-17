/*
 * mcu_gpio.h
 *
 *  Created on: 2023 Mar 13
 *      Author: ruffman
 */

#ifndef INCLUDE_MCU_GPIO_H_
#define INCLUDE_MCU_GPIO_H_


#ifdef AT_START_F421_V1
#include "at32f421_board.h"
#endif

#ifdef AT_START_F415_V1
#include "at32f415_board.h"
#endif

void gpio_on(gpio_type* port, uint16_t pin);
void gpio_off(gpio_type* port, uint16_t pin);
void gpio_toggle(gpio_type* port, uint16_t pin);

void init_gpio_input(
		crm_periph_clock_type crm_clock,
		gpio_type* port,
		uint16_t pin,
		gpio_pull_type pull);
void init_gpio_output(
		crm_periph_clock_type crm_clock,
		gpio_type* port,
		uint16_t pin,
		gpio_pull_type pull);



#endif /* INCLUDE_MCU_GPIO_H_ */
