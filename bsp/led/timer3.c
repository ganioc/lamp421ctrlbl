/*
 * timer3.c
 *
 *  Created on: 2023 Mar 13
 *      Author: ruffman
 */


#include "at32f421_board.h"


// 1s
//#define TIMER3_COUNT_VAL
// 1/600, 600 bps, 109
// 1/300,  300 bps,
#define TIMER3_COUNT_VAL        109

// #define AT_TIMEOUT_COUNTER    (AT_TIMEOUT_DUR*65535/(1000*109))

uint16_t    at_counter = 0;

//void start_timer(){
//	at_counter = 0;
//	AT_in_handling = 1;
//}
//void stop_timer(){
//	AT_in_handling = 0;
//}
//void check_at_timeout(){
//	if(AT_in_handling == 1){
//		at_counter++;
//
//		if(at_counter >= AT_TIMEOUT_COUNTER){
//			parserTimerCallback();
//			AT_in_handling = 0;
//		}
//	}
//
//}

void timer3_config(){
	crm_clocks_freq_type crm_clocks_freq_struct = {0};
	/* get system clock */
	crm_clocks_freq_get(&crm_clocks_freq_struct);
	/* enable tmr1 clock */
	crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);

	/* tmr1 configuration */
	/* time base configuration */
	/* systemclock/12000/10000 = 1hz */
	tmr_base_init(TMR3, TIMER3_COUNT_VAL, (crm_clocks_freq_struct.ahb_freq / 65535) - 1);
	tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);

	/* overflow interrupt enable */
	tmr_interrupt_enable(TMR3, TMR_OVF_INT, TRUE);
	/* tmr1 overflow interrupt nvic init */
	nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
	nvic_irq_enable(TMR3_GLOBAL_IRQn, 0, 0);
	/* enable tmr3 */
	tmr_counter_enable(TMR3, TRUE);
}


/**
  * @brief  this function handles tmr3 global interrupt request.
  * @param  none
  * @retval none
  */
void TMR3_GLOBAL_IRQHandler(void) {
	if (tmr_flag_get(TMR3, TMR_OVF_FLAG) != RESET) {
		tmr_flag_clear(TMR3, TMR_OVF_FLAG);
		simu_uart_tx();
		// led_eth_toggle();

		// check_at_timeout();
	}
}


