/*
 * iap_gpio.c
 *
 *  Created on: 2023 Mar 13
 *      Author: ruffman
 */


#include "iap_gpio.h"

void init_iap_gpio(){
	init_gpio_input(
			IAP_CRM_CLOCK,
			IAP_PORT,
			IAP_PIN,
			GPIO_PULL_UP);
}
