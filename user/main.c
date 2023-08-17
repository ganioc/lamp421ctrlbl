/**
 **************************************************************************
 * @file     main.c
 * @version  v2.0.6
 * @date     2022-05-20
 * @brief    main program
 **************************************************************************
 *                       Copyright notice & Disclaimer
 *
 * The software Board Support Package (BSP) that is made available to
 * download from Artery official website is the copyrighted work of Artery.
 * Artery authorizes customers to use, copy, and distribute the BSP
 * software and its related documentation for the purpose of design and
 * development in conjunction with Artery microcontrollers. Use of the
 * software is governed by this copyright notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
 * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
 * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
 * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
 * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 *
 **************************************************************************
 */

#include "at32f421_board.h"
#include "at32f421_clock.h"

#include "d1_uart.h"
#include "flash.h"
#include "iap_gpio.h"
#include "ymodem.h"
#include "iap.h"

/**
 * @brief  main function.
 * @param  none
 * @retval none
 */
int main(void) {

	COM_StatusTypeDef result;
	int target_delay_times = 3;

	system_clock_config();

	at32_board_init();

	init_d1_uart();

	led_gpio_init();

	init_iap_gpio();

	timer3_config();

	delay_ms(500);

	// button_exint_init();

	print_welcome();

	cust_printf("Flash size : %d\r\n", FLASH_SIZE);


	while (target_delay_times > 0) {

		if (SET == gpio_input_data_bit_read(IAP_PORT, IAP_PIN)){
			cust_printf("iap port: 1\r\n");
			break;

		}else{
			target_delay_times--;
			cust_printf("iap port: 0\r\n");
			delay_ms(20);
		}
	}
	cust_printf("delay_times: %d\r\n", target_delay_times);

	delay_ms(250);

	if(target_delay_times == 0){
		led_run_on();

		result = SerialDownload();
		cust_printf("result of donwload serial\r\n", result);
		if (result == COM_OK) {
			cust_printf("Download succeed\r\n");
		}else{
			cust_printf("Download failed\r\n");
		}
	}
	delay_ms(500);

	/* check app starting address whether 0x08xxxxxx */
	if (((*(uint32_t*) (APP_START_ADDR + 4)) & 0xFF000000) == 0x08000000){
		cust_printf("Jump to app\r\n");
		app_load(APP_START_ADDR);
	}else{
		cust_printf("Can not jump to app\r\n");
	}


	while (1) {
		delay_ms(100);
		led_run_toggle();
		cust_printf("Still in boot\r\n");

	}
}

/**
 * @}
 */

/**
 * @}
 */
