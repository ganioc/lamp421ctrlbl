/*
 * iap_gpio.h
 *
 *  Created on: 2023 Mar 13
 *      Author: ruffman
 */

#ifndef BSP_IAP_IAP_GPIO_H_
#define BSP_IAP_IAP_GPIO_H_

#ifdef AT_START_F421_V1
#include "at32f421_board.h"
#endif

#ifdef AT_START_F415_V1
#include "at32f415_board.h"
#endif

/********   Download Indication GPIO   ********/
#define IAP_PIN         GPIO_PINS_7
#define IAP_PORT        GPIOA
#define IAP_CRM_CLOCK   CRM_GPIOA_PERIPH_CLOCK


void init_iap_gpio();

#endif /* BSP_IAP_IAP_GPIO_H_ */
