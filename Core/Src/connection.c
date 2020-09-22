/*
 * mainApp.c
 *
 *  Created on: Feb 4, 2020
 *      Author: G K Praful and Sonal kumar
 */

#include "connection.h"
#include "rfm98.h"
#include "string.h"
#include "main.h"

char strBuf[64];
u16 SysTime;
u16 time2_count;
u16 key1_time_count;
u16 key2_time_count;
u8 rf_rx_packet_length;

u8 mode;//lora--1/FSK--0
u8 Freq_Sel;//
u8 Power_Sel;//
u8 Lora_Rate_Sel;//
u8 BandWide_Sel;//

u8 Fsk_Rate_Sel;//

u8 key1_count;
u8 time_flag;
u8	operation_flag;
u8 key_flag;
//static const uint8_t TMP102_ADDR = 0x48<<1;
//static const uint8_t REG_TEMP = 0x00;
I2C_HandleTypeDef hi2c1;

//SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;


void connection()
//void connection(float* transfer)
{
	u16 i=0;//,j,k=0,g;
	//u16 k=(float*)transfer;
	SysTime = 0;
	//HAL_StatusTypeDef ret;
	float tempp;
	uint8_t buff[12];
	//	int16_t val;
	//	float temp_c;

	operation_flag = 0x00;
	key1_count = 0x00;
	mode = 0x01;//lora mode
	Freq_Sel = 0x00;//433M
	Power_Sel = 0x00;//
	Lora_Rate_Sel = 0x06;//
	BandWide_Sel = 0x07;
	Fsk_Rate_Sel = 0x00;

	RED_LED_L();
	HAL_Delay(1000);
	RED_LED_H();


	HAL_GPIO_WritePin(Reset_GPIO_Port,Reset_Pin,GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(Reset_GPIO_Port,Reset_Pin,GPIO_PIN_SET);

	rfm98_Config();//
	rfm98_LoRaEntryRx();


	while (1)
	{
		key1_count = 0; //0 -transmitter  1 -receiver

		 tempp=tmp102();

		 sprintf(buff, "%f"   ,tempp);

			  HAL_UART_Transmit(&huart2, buff, strlen((char*)buff), HAL_MAX_DELAY);
				  HAL_Delay(500);


		switch(key1_count)
		{
			case 0://lora Tx

				//if(time_flag & 0x02)//2s time
				{
				//	time_flag &= 0xfd;
					sprintf((char*)rfm98Data,"%s  ",buff);  //k);
					//printUSB(rfm98Data);
					RED_LED_H();


					i++;
					rfm98_LoRaEntryTx();
					rfm98_LoRaTxPacket();

					RED_LED_L();
					HAL_Delay(1000);

					//rfm98_LoRaEntryRx();
				}
//				if(rfm98_LoRaRxPacket())
	//			{
//
	//				HAL_Delay(100);

		//		}

			break;
			case 1://lora slaver Rx

				if(rfm98_LoRaRxPacket())
				{
					sprintf(strBuf,"Receive Data %s\n",(char*)RxData);
					//printUSB(strBuf);
					RED_LED_H();
					HAL_Delay(500);
//					rfm98_LoRaEntryTx();
//					rfm98_LoRaTxPacket();
					RED_LED_L();
//					rfm98_LoRaEntryRx();

				}


			break;
			case 2:


			break;
			case 3:


			break;
			default:

			break;
		}
	}
}


