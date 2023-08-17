/*
 * flash.h
 *
 *  Created on: 2023 Mar 13
 *      Author: ruffman
 */

#ifndef INCLUDE_FLASH_H_
#define INCLUDE_FLASH_H_

#include "at32f421_board.h"

/** @addtogroup AT32F421_periph_examples
  * @{
  */

/** @addtogroup 421_FLASH_write_read
  * @{
  */


/** @defgroup FLASH_write_read_functions
  * @{
  */
#define FLASH_SIZE    (*((uint32_t*)0x1FFFF7E0))
/* read from at32 flash capacity register(unit:kbyte) */
#define SRAM_SIZE                       224
/* sram size, unit:kbyte */

/*   sys state     */
#define EEPROM_FLASH_ADDRESS_START       (0x08000000 + 1024 * 60)

#define SECTOR_SIZE                      1024

#define PARAM_PWM_ADDRESS   (EEPROM_FLASH_ADDRESS_START)
#define PARAM_VO_ADDRESS    (EEPROM_FLASH_ADDRESS_START + 1*SECTOR_SIZE)
#define PARAM_MAGIC         0x1E1F

void flash_2kb_write(uint32_t write_addr, uint8_t *pbuffer);

void flash_read(uint32_t read_addr, uint16_t *p_buffer, uint16_t num_read);
error_status flash_write_nocheck(uint32_t write_addr, uint16_t *p_buffer, uint16_t num_write);
error_status flash_write(uint32_t write_addr,uint16_t *p_Buffer, uint16_t num_write);

#endif /* INCLUDE_FLASH_H_ */
