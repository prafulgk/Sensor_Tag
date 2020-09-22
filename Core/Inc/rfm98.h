/*
 * rfm98.h
 *
 *  Created on: Feb 4, 2020
 *      Author: G K Praful and Sonal kumar
 */


#ifndef RFM98_
#define RFM98_

#include "main.h"
#include "stm32l4xx_hal.h"
#include "My_type.h"
#include "spi.h"

/*****************IO define********************/
#define RED_LED_H()					HAL_GPIO_WritePin(RED_LED_GPIO_Port,RED_LED_Pin,GPIO_PIN_SET)//PD_ODR |= 0x40//PD6
#define RED_LED_L()					HAL_GPIO_WritePin(RED_LED_GPIO_Port,RED_LED_Pin,GPIO_PIN_RESET)//PD_ODR &= 0xbf

#define Get_KEY2()					(PD_IDR & 0x10) == 0x10//PD4
#define Get_KEY1()					(PD_IDR & 0x08) == 0x08//PD3

///////////////////////////////////////////////// LoRa mode //////////////////////////////////////////////////
//Error Coding rate (CR)setting
#define CR_4_5
//#define CR_4_6    0
//#define CR_4_7    0
//#define CR_4_8    0

#ifdef   CR_4_5
  #define CR    0x01                                       // 4/5
#else
  #ifdef   CR_4_6
    #define CR    0x02                                     // 4/6
  #else
    #ifdef   CR_4_7
      #define CR    0x03                                   // 4/7
    #else
      #ifdef   CR_4_8
        #define CR    0x04                                 // 4/8
      #endif
    #endif
  #endif
#endif

//CRC Enable
#define SPI_CRC_EN

#ifdef  SPI_CRC_EN
  #define SPI_CRC   0x01                                       //CRC Enable
#else
  #define SPI_CRC   0x00
#endif

//RFM98 Internal registers Address
/********************Lroa mode***************************/
#define LR_RegFifo                                  0x00
// Common settings
#define LR_RegOpMode                                0x01
#define LR_RegFrMsb                                 0x06
#define LR_RegFrMid                                 0x07
#define LR_RegFrLsb                                 0x08
// Tx settings
#define LR_RegPaConfig                              0x09
#define LR_RegPaRamp                                0x0A
#define LR_RegOcp                                   0x0B
// Rx settings
#define LR_RegLna                                   0x0C
// LoRa registers
#define LR_RegFifoAddrPtr                           0x0D
#define LR_RegFifoTxBaseAddr                        0x0E
#define LR_RegFifoRxBaseAddr                        0x0F
#define LR_RegFifoRxCurrentaddr                     0x10
#define LR_RegIrqFlagsMask                          0x11
#define LR_RegIrqFlags                              0x12
#define LR_RegRxNbBytes                             0x13
#define LR_RegRxHeaderCntValueMsb                   0x14
#define LR_RegRxHeaderCntValueLsb                   0x15
#define LR_RegRxPacketCntValueMsb                   0x16
#define LR_RegRxPacketCntValueLsb                   0x17
#define LR_RegModemStat                             0x18
#define LR_RegPktSnrValue                           0x19
#define LR_RegPktRssiValue                          0x1A
#define LR_RegRssiValue                             0x1B
#define LR_RegHopChannel                            0x1C
#define LR_RegModemConfig1                          0x1D
#define LR_RegModemConfig2                          0x1E
#define LR_RegSymbTimeoutLsb                        0x1F
#define LR_RegPreambleMsb                           0x20
#define LR_RegPreambleLsb                           0x21
#define LR_RegPayloadLength                         0x22
#define LR_RegMaxPayloadLength                      0x23
#define LR_RegHopPeriod                             0x24
#define LR_RegFifoRxByteAddr                        0x25

// I/O settings
#define REG_LR_DIOMAPPING1                          0x40
#define REG_LR_DIOMAPPING2                          0x41
// Version
#define REG_LR_VERSION                              0x42
// Additional settings
#define REG_LR_PLLHOP                               0x44
#define REG_LR_TCXO                                 0x4B
#define REG_LR_PADAC                                0x4D
#define REG_LR_FORMERTEMP                           0x5B

#define REG_LR_AGCREF                               0x61
#define REG_LR_AGCTHRESH1                           0x62
#define REG_LR_AGCTHRESH2                           0x63
#define REG_LR_AGCTHRESH3                           0x64

/********************FSK/ook mode***************************/
#define  RegFIFO  	 				0x00				//FIFO
#define  RegOpMode 	 				0x01
#define  RegBitRateMsb 				0x02
#define  RegBitRateLsb 				0x03
#define  RegFdevMsb	 				0x04
#define  RegFdevLsb	 				0x05
#define  RegFreqMsb	 				0x06
#define  RegFreqMid	 				0x07
#define  RegFreqLsb   				0x08
#define	 RegPaConfig				0x09
#define  RegPaRamp					0x0a
#define  RegOcp						0x0b
#define  RegLna						0x0c
#define  RegRxConfig				0x0d
#define  RegRssiConfig				0x0e
#define  RegRssiCollision 			0x0f
#define  RegRssiThresh				0x10
#define  RegRssiValue				0x11
#define  RegRxBw					0x12
#define  RegAfcBw					0x13
#define  RegOokPeak					0x14
#define  RegOokFix					0x15
#define  RegOokAvg					0x16

#define  RegAfcFei					0x1a
#define  RegAfcMsb					0x1b
#define  RegAfcLsb					0x1c
#define  RegFeiMsb					0x1d
#define  RegFeiLsb					0x1e
#define  RegPreambleDetect			0x1f
#define  RegRxTimeout1				0x20
#define  RegRxTimeout2				0x21
#define  RegRxTimeout3				0x22
#define  RegRxDelay					0x23
#define  RegOsc  	 				0x24
#define  RegPreambleMsb				0x25
#define  RegPreambleLsb				0x26
#define  RegSyncConfig				0x27
#define  RegSyncValue1				0x28
#define  RegSyncValue2				0x29
#define  RegSyncValue3				0x2a
#define  RegSyncValue4				0x2b
#define  RegSyncValue5				0x2c
#define  RegSyncValue6				0x2d
#define  RegSyncValue7				0x2e
#define  RegSyncValue8				0x2f
#define  RegPacketConfig1			0x30
#define  RegPacketConfig2			0x31
#define  RegPayloadLength			0x32
#define  RegNodeAdrs				0x33
#define  RegBroadcastAdrs			0x34
#define  RegFifoThresh				0x35
#define  RegSeqConfig1				0x36
#define  RegSeqConfig2				0x37
#define  RegTimerResol				0x38
#define  RegTimer1Coef				0x39
#define  RegTimer2Coef				0x3a
#define  RegImageCal				0x3b
#define  RegTemp					0x3c
#define  RegLowBat					0x3d
#define  RegIrqFlags1				0x3e
#define  RegIrqFlags2				0x3f
#define  RegDioMapping1				0x40
#define  RegDioMapping2				0x41
#define  RegVersion					0x42

#define	 RegPllHop					0x44
#define  RegPaDac					0x4d
#define	 RegBitRateFrac				0x5d
/*********************************************************/
//command
/*********************************************************/
extern u8 key_flag;
/*{
	uchar	:key1_shot_down;
	uchar	:key1_long_down;
	uchar	:key2_short_down;
	uchar	:key2_long_down
	uchar	:
	uchar	:;
	uchar	:;
	uchar	;
}*/
extern u8 mode;
extern u8 Freq_Sel;
extern u8 Power_Sel;
extern u8 Lora_Rate_Sel;
extern u8 BandWide_Sel;
extern u8 Fsk_Rate_Sel;
extern u16 SysTime;
extern u8  rfm98Data[64];
extern void delay_ms(unsigned int ms);
extern void delay_us(unsigned int us);


extern u8 RxData[64];
extern void rfm98_Config(void);
extern u8 rfm98_LoRaEntryRx(void);
extern u8 rfm98_LoRaReadRSSI(void);
extern u8 rfm98_LoRaRxPacket(void);
extern u8 rfm98_LoRaEntryTx(void);
extern u8 rfm98_LoRaTxPacket(void);

#endif /* rfm98_H_ */
