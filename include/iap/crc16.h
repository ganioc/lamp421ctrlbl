/*
 * crc16.h
 *
 *  Created on: 2023 Mar 13
 *      Author: ruffman
 */

#ifndef INCLUDE_IAP_CRC16_H_
#define INCLUDE_IAP_CRC16_H_

#ifdef AT_START_F421_V1
#include "at32f421_board.h"
#endif

#ifdef AT_START_F415_V1
#include "at32f415_board.h"
#endif

uint16_t Cal_CRC16(const uint8_t* data, uint32_t size);

uint32_t Str2Int(uint8_t *p_inputstr, uint32_t *p_intnum);

#endif /* INCLUDE_IAP_CRC16_H_ */
