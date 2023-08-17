/*
 * led.c
 *
 *  Created on: 2023 Mar 13
 *      Author: ruffman
 */


#include "at32f421_board.h"

#include "mcu_gpio.h"

void led_gpio_init(){
	init_gpio_output(LED_RUN_GPIO_CLK,
			LED_RUN_GPIO_PORT,
			LED_RUN_PIN,
			GPIO_PULL_UP);
	init_gpio_output(LED_ETH_GPIO_CLK,
			LED_ETH_GPIO_PORT,
			LED_ETH_PIN,
			GPIO_PULL_UP);
	init_gpio_output(LED_NET_GPIO_CLK,
			LED_NET_GPIO_PORT,
			LED_NET_PIN,
			GPIO_PULL_UP);

}
void led_run_on(){
	gpio_on(LED_RUN_GPIO_PORT,
			LED_RUN_PIN);
}
void led_run_off(){
	gpio_off(LED_RUN_GPIO_PORT,
			LED_RUN_PIN);
}
void led_run_toggle(){
	gpio_toggle(LED_RUN_GPIO_PORT,
			LED_RUN_PIN);
}
void led_eth_on(){
	gpio_on(LED_ETH_GPIO_PORT,
			LED_ETH_PIN);
}
void led_eth_off(){
	gpio_off(LED_ETH_GPIO_PORT,
			LED_ETH_PIN);
}
void led_eth_toggle(){
	gpio_toggle(LED_ETH_GPIO_PORT,
			LED_ETH_PIN);
}
void led_net_on(){
	gpio_on(LED_NET_GPIO_PORT,
			LED_NET_PIN);
}
void led_net_off(){
	gpio_off(LED_NET_GPIO_PORT,
			LED_NET_PIN);
}
void led_net_toggle(){
	gpio_toggle(LED_NET_GPIO_PORT,
			LED_NET_PIN);
}

