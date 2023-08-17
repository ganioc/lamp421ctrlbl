/*
 * ymodem.c
 *
 *  Created on: 2023 Mar 13
 *      Author: ruffman
 */



#include "ymodem.h"
#include "crc16.h"
#include "iap.h"

#define     USE_DEBUG_YMODEM

uint16_t aPacketSize = 0;
uint8_t aPacketData[PACKET_1K_SIZE +
						   PACKET_DATA_INDEX +
						   PACKET_TRAILER_SIZE];
static uint8_t file_name[FILE_NAME_LENGTH];
static uint8_t file_size[FILE_SIZE_LENGTH];
static uint8_t eeprom_buf[SECTOR_SIZE];
//uint8_t* eeprom[16];



uint16_t packet_size;
PacketTypeDef  packet_type;

// printRXBuf
void printRXBuf(uint8_t *buffer, int len) {
//	char str[64];
//	char temp[8];
//    PRINT_LOG("printRxBuf: %d\r\n" , len);
//    int upper;
//
//
//    for (int i = 0; i < len; i += 16) {
//      PRINT_LOG("%02x: ", i);
//
//      upper = (len < i + 16) ? len : i + 16;
//
//      for (int j = i; j < upper; j++) {
//          PRINT_LOG("%02x ", buffer[j]);
//      }
//      PRINT_LOG("\r\n");
//    }
}

COM_StatusTypeDef UART_ReceiveEx(uint8_t *pData, uint16_t size){
	uint8_t val;
	uint16_t index=0, crc , crc2;
	uint16_t counter = UART_RX_COUNTER;

	while(counter--){
		//delay_ms(UART_DELAY_DUR);
		if(aPacketSize > 0){
			break;
		}
		delay_ms(UART_DELAY_DUR);
	}
	if(counter <= 0){
		return COM_TIMEOUT;
	}
//	PRINT_LOG("\r\n<--\r\n");
	PRINT_LOG("\r\n<--%d\r\n", aPacketSize);
	printRXBuf(pData, aPacketSize);

	val = pData[0];

	switch(val){
	case SOH: packet_size = 128;
		packet_type = PACKET_SOH;
		PRINT_LOG("SOH\r\n");
		break;
	case STX: packet_size = 1024;
		packet_type = PACKET_STX;
		PRINT_LOG("STK\r\n");
		break;
	case EOT: packet_size = 1;
		packet_type = PACKET_EOT;
		PRINT_LOG("EOT\r\n");
		break;
	case CA:
		if(pData[1] == CA){
			packet_size = 2;
			packet_type = PACKET_CA;
			PRINT_LOG("CA\r\n");
		}else{
			return COM_ERROR;
		}
		break;
	case ABORT1:
	case ABORT2:
		return COM_BUSY;
		break;
	default:
		return COM_ERROR;
		break;
	}

	if(packet_size >= 128){
		if(pData[1] != (0xFF - pData[2])){
			PRINT_LOG("id error\r\n");
			packet_size = 0;
			return COM_ERROR;
		}else{// compute CRC
			crc = pData[packet_size + 3] << 8;
			crc += pData[packet_size + 4];
			crc2 = Cal_CRC16(&pData[3], packet_size);
			//PRINT_LOG("crc: %02x, crc2: %02x\r\n", crc, crc2);

			if(crc != crc2){
				PRINT_LOG("crc not match!\r\n");
				packet_size = 0;
				return COM_ERROR;
			}else{
				//PRINT_LOG("crc match!\r\n");
			}
		}
	}

	return COM_OK;

}
void extract_file_name_size(uint8_t *buffer){
	  uint16_t index = 3;

	  uint16_t i = 0;

	  while(buffer[index] != 0){
	    file_name[i++] = buffer[index++];
	  }
	  file_name[i] = 0;

	  i = 0;
	  index++;
	  while(buffer[index] != 0){
	    file_size[i++] = buffer[index++];
	  }
	  file_size[i] = 0;

//	  PRINT_LOG("file name, file size:\r\n");
//	  PRINT_LOG("%s\r\n", file_name);
//	  PRINT_LOG("%s\r\n", file_size);
}

void send_response(uint8_t ch, int len){
	PRINT_LOG("r-->\r\n");
	uint8_t temp[3];
	int i;
	for(i=0;i<len;i++){
		temp[i] = ch;
//		PRINT_LOG("%02x ", temp[i]);
	}
//	PRINT_LOG("\r\n");
	data_send(temp,len);
}

COM_StatusTypeDef SerialDownload(){
	uint8_t session_done = 0;
	COM_StatusTypeDef  result = COM_OK, pkt_result;
	uint8_t session_begin = 0;
	uint16_t blocks = 0;
	uint8_t  eeprom_index = 0;
	uint8_t  eeprom_pos_index = 0;
	error_status eeprom_status;
	uint16_t  block_length = 0;

	PRINT_LOG("SD\r\n");


	while (session_done == 0 && result == COM_OK) {
		uint16_t packets_received = 0;
		uint16_t file_done = 0;
		uint16_t inRx_block0 = 0;
		uint16_t inEOT = 0;
		uint16_t errors = 0;

		while (file_done == 0 && result == COM_OK) {
			aPacketSize = 0;

			pkt_result = UART_ReceiveEx(aPacketData, 1024);
			//PRINT_LOG("pkt_result: %d\r\n", pkt_result);

			if (pkt_result == COM_OK) {
				//PRINT_LOG("RX valid packet\r\n");
				errors = 0;

				switch(packet_type){
				case PACKET_CA:
					send_response(ACK,1);
					result = COM_ABORT;
					break;
				case PACKET_EOT:
					if(inEOT == 0){
						send_response(NAK,1);
						inEOT = 1;
					}else if(inEOT == 1){
						send_response(ACK, 1);
						delay_ms(50);
						send_response(CRC16, 1);
					}
					break;
				default:
//					PRINT_LOG("\r\nNormal packet: %02x\r\n", aPacketData[1]);
//					PRINT_LOG("packets received: %d\r\n", packets_received);

					if(packet_type == PACKET_SOH){
						block_length = 128;
					}else{
						block_length = 1024;
					}


					if((aPacketData[1] != (packets_received%256)) && inEOT == 0){
						PRINT_LOG("Packet id nm, %d != %d\r\n",
								aPacketData[1], packets_received%256);
						send_response(NAK, 1);
					}else{
						// first block , block 0
						if(packets_received == 0 && inRx_block0 == 0){
							// file name extraction
							// file size extraction
							PRINT_LOG("Received 1st block 0");
							//printRXBuf(aPacketData, 128 + 5);
							extract_file_name_size(aPacketData);

							// PRINT_LOG("erase the blank sector\r\n");
							send_response(ACK, 1);
							send_response(CRC16	, 1);
							inRx_block0 = 1;
						}else if(packets_received != 0 && inRx_block0 == 1 && inEOT == 1){
							PRINT_LOG("Received last block 0, meaning END\r\n");
							PRINT_LOG("Copy eeprom to flash: %d\r\n", eeprom_pos_index);
							eeprom_status =  write_to_app_start(eeprom_pos_index, eeprom_buf);
							PRINT_LOG("eeprom_status :%d\r\n", eeprom_status);

							send_response(ACK, 1);
							file_done = 1;
							session_done = 1;
							return COM_OK;

						}else{ // Other blocks , data packets
							// 128 bytes block
							//1024 bytes block
							// PRINT_LOG("write to EEPROM\r\n");
							//printRXBuf(aPacketData, 128 + 5);
							blocks++;
							PRINT_LOG("blocks:%d\r\n", blocks);

							// copy block *4 into eeprom_buf,
							// and then save it to the flash;
							PRINT_LOG("eeprom_index: %d\r\n", eeprom_index);

							for(int i = 0; i< block_length; i++){
								eeprom_buf[eeprom_index*block_length + i] = aPacketData[3+i];
							}

							eeprom_index++;

							if(eeprom_index >= (SECTOR_SIZE/block_length)){
								// save to eeprom
								LED_INDICATION();
								PRINT_LOG("copy eeprom to flash: %d\r\n", eeprom_pos_index);

								eeprom_status = write_to_app_start(eeprom_pos_index, eeprom_buf);

								PRINT_LOG("eeprom_status :%d\r\n", eeprom_status);

								eeprom_index = 0;
								eeprom_pos_index++;

							}

							send_response(ACK, 1);
						}
						packets_received++;
						PRINT_LOG("received: %d\r\n", packets_received);
						session_begin = 1;
					}
					break;
				}

			} else if (pkt_result == COM_BUSY) {
				send_response(CA, 2);
				result = COM_ABORT;
			} else {
				if (session_begin > 0) {
					errors++;
				}
				if (errors > MAX_ERRORS) {
					send_response(CA, 2);
					result = COM_ABORT;
					return result;
				} else {
					PRINT_LOG("Wait for host response ...\r\n");
					send_response(CRC16, 1);
				}
			}
		}
	}
	return result;
}
