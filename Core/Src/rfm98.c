/*
 * rfm98.c
 *
 *  Created on: Feb 4, 2020
 *      Author: G K Praful and Sonal Kumar
 */

#include "rfm98.h"
#include "spi.h"

/************************Description************************

	STM32F103C8t6		LORA RA-01
		PA7			-	MOSI - 14
		PA6			-	MISO - 13
		PA5			-	SCK - 12
		PA4			- 	NSS - 15
		PA3			-	DIO1 - 6
		PA2			-	DIO0 - 5
		PA1			-	RESET - 4
		3.3v		-	3.3v - 3
		GND			- 	GND-2,9,16

************************************************/
/************************************************
//  RF module:           rfm98
//  FSK:
//  Carry Frequency:     434MHz
//  Bit Rate:            1.2Kbps/2.4Kbps/4.8Kbps/9.6Kbps
//  Tx Power Output:     20dbm/17dbm/14dbm/11dbm
//  Frequency Deviation: +/-35KHz
//  Receive Bandwidth:   83KHz
//  Coding:              NRZ
//  Packet Format:       0x5555555555+0xAA2DD4+"Mark1 Lora rfm98" (total: 29 bytes)
//  LoRa:
//  Carry Frequency:     434MHz
//  Spreading Factor:    6/7/8/9/10/11/12
//  Tx Power Output:     20dbm/17dbm/14dbm/11dbm
//  Receive Bandwidth:   7.8KHz/10.4KHz/15.6KHz/20.8KHz/31.2KHz/41.7KHz/62.5KHz/125KHz/250KHz/500KHz
//  Coding:              NRZ
//  Packet Format:       "Mark1 Lora rfm98" (total: 21 bytes)
//  Tx Current:          about 120mA  (RFOP=+20dBm,typ.)
//  Rx Current:          about 11.5mA  (typ.)
**********************************************************/

/**********************************************************
**Parameter table define
**********************************************************/
const u8 rfm98FreqTbl[1][3] =
{
  {0x6C, 0x80, 0x00}, //434MHz
};

const u8 rfm98PowerTbl[4] =
{
  0xFF,                   //20dbm
  0xFC,                   //17dbm
  0xF9,                   //14dbm
  0xF6,                   //11dbm
};

const u8 rfm98SpreadFactorTbl[7] =
{
  6,7,8,9,10,11,12
};

const u8 rfm98LoRaBwTbl[10] =
{
//7.8KHz,10.4KHz,15.6KHz,20.8KHz,31.2KHz,41.7KHz,62.5KHz,125KHz,250KHz,500KHz
  0,1,2,3,4,5,6,7,8,9
};

u8  rfm98Data[64] = {"TX_WT Lora rfm98"};
//const u8  rfm98Data[] = {"Mark1 Lora rfm98"};

u8 RxData[64];

/**********************************************************
**Variable define
**********************************************************/

void rfm98_Config(void);

/**********************************************************
**Name:     rfm98_Standby
**Function: Entry standby mode
**Input:    None
**Output:   None
**********************************************************/
void rfm98_Standby(void)
{
  SPIWrite(LR_RegOpMode,0x09);                              		//Standby//Low Frequency Mode
	//SPIWrite(LR_RegOpMode,0x01);                              	 //Standby//High Frequency Mode
}

/**********************************************************
**Name:     rfm98_Sleep
**Function: Entry sleep mode
**Input:    None
**Output:   None
**********************************************************/
void rfm98_Sleep(void)
{
  SPIWrite(LR_RegOpMode,0x08);                              		//Sleep//Low Frequency Mode
	//SPIWrite(LR_RegOpMode,0x00);                            		 //Sleep//High Frequency Mode
}

/*********************************************************/
//LoRa mode
/*********************************************************/
/**********************************************************
**Name:     rfm98_EntryLoRa
**Function: Set RFM69 entry LoRa(LongRange) mode
**Input:    None
**Output:   None
**********************************************************/
void rfm98_EntryLoRa(void)
{
  SPIWrite(LR_RegOpMode,0x88);//Low Frequency Mode
	//SPIWrite(LR_RegOpMode,0x80);//High Frequency Mode
}

/**********************************************************
**Name:     rfm98_LoRaClearIrq
**Function: Clear all irq
**Input:    None
**Output:   None
**********************************************************/
void rfm98_LoRaClearIrq(void)
{
  SPIWrite(LR_RegIrqFlags,0xFF);
}

/**********************************************************
**Name:     rfm98_LoRaEntryRx
**Function: Entry Rx mode
**Input:    None
**Output:   None
**********************************************************/
u8 rfm98_LoRaEntryRx(void)
{
  u8 addr;

  rfm98_Config();                                         //setting base parameter

  SPIWrite(REG_LR_PADAC,0x84);                              //Normal and Rx
  SPIWrite(LR_RegHopPeriod,0xFF);                          //RegHopPeriod NO FHSS
  SPIWrite(REG_LR_DIOMAPPING1,0x01);                       //DIO0=00, DIO1=00, DIO2=00, DIO3=01

  SPIWrite(LR_RegIrqFlagsMask,0x3F);                       //Open RxDone interrupt & Timeout
  rfm98_LoRaClearIrq();
  SPIWrite(0x30,1<<7);
  SPIWrite(LR_RegPayloadLength,21);                       //RegPayloadLength  21byte(this register must difine when the data long of one byte in SF is 6)

  addr = SPIRead(LR_RegFifoRxBaseAddr);           				//Read RxBaseAddr
  SPIWrite(LR_RegFifoAddrPtr,addr);                        //RxBaseAddr -> FiFoAddrPtr��
  SPIWrite(LR_RegOpMode,0x8d);                        		//Continuous Rx Mode//Low Frequency Mode
	//SPIWrite(LR_RegOpMode,0x05);                        		//Continuous Rx Mode//High Frequency Mode
	SysTime = 0;
	while(1)
	{
		if((SPIRead(LR_RegModemStat)&0x04)==0x04)   //Rx-on going RegModemStat
			break;
		if(SysTime>=3)
			return 0;                                              //over time for error
	}
	return 0;
}
/**********************************************************
**Name:     rfm98_LoRaReadRSSI
**Function: Read the RSSI value
**Input:    none
**Output:   temp, RSSI value
**********************************************************/
u8 rfm98_LoRaReadRSSI(void)
{
  u16 temp=10;
  temp=SPIRead(LR_RegRssiValue);                  //Read RegRssiValue��Rssi value
  temp=temp+127-137;                                       //127:Max RSSI, 137:RSSI offset
  return (u8)temp;
}

/**********************************************************
**Name:     rfm98_LoRaRxPacket
**Function: Receive data in LoRa mode
**Input:    None
**Output:   1- Success
            0- Fail
**********************************************************/
u8 rfm98_LoRaRxPacket(void)
{
  u8 i;
  u8 addr;
  u8 packet_size;

  if(Get_NIRQ())
  {
    for(i=0;i<32;i++)
      RxData[i] = 0x00;

    addr = SPIRead(LR_RegFifoRxCurrentaddr);      //last packet addr
    SPIWrite(LR_RegFifoAddrPtr,addr);                      //RxBaseAddr -> FiFoAddrPtr
    if(rfm98SpreadFactorTbl[Lora_Rate_Sel]==6)           //When SpreadFactor is six��will used Implicit Header mode(Excluding internal packet length)
      packet_size=21;
    else
      packet_size = SPIRead(LR_RegRxNbBytes);     //Number for received bytes
    SPIBurstRead(0x00, RxData, packet_size);

    rfm98_LoRaClearIrq();
//    for(i=0;i<17;i++)
//    {
//      if(RxData[i]!=rfm98Data[i])
//        break;
//    }
//    if(i>=17)                                              //Rx success
//      return(1);
//    else
//      return(0);
  }
  else
    return(0);
  return(1);
}

/**********************************************************
**Name:     rfm98_LoRaEntryTx
**Function: Entry Tx mode
**Input:    None
**Output:   None
**********************************************************/
u8 rfm98_LoRaEntryTx(void)
{
  u8 addr,temp;

  rfm98_Config();                                         //setting base parameter

  SPIWrite(REG_LR_PADAC,0x87);                                   //Tx for 20dBm
  SPIWrite(LR_RegHopPeriod,0x00);                               //RegHopPeriod NO FHSS
  SPIWrite(REG_LR_DIOMAPPING1,0x41);                   	    //DIO0=01, DIO1=00, DIO2=00, DIO3=01

  rfm98_LoRaClearIrq();
  SPIWrite(LR_RegIrqFlagsMask,0xF7);                       	//Open TxDone interrupt
  SPIWrite(LR_RegPayloadLength,6);                       	//RegPayloadLength  __byte

  addr = SPIRead(LR_RegFifoTxBaseAddr);           			//RegFiFoTxBaseAddr
  SPIWrite(LR_RegFifoAddrPtr,addr);                        	//RegFifoAddrPtr
	SysTime = 0;
	while(1)
	{
		temp=SPIRead(LR_RegPayloadLength);
		if(temp==6)
		{
			break;
		}
		if(SysTime>=3)
			return 0;
	}
	return 0;
}
/**********************************************************
**Name:     rfm98_LoRaTxPacket
**Function: Send data in LoRa mode
**Input:    None
**Output:   1- Send over
**********************************************************/
u8 rfm98_LoRaTxPacket(void)
{
 // u8 TxFlag=0;
 // u8 addr;

	BurstWrite(0x00, (u8 *)rfm98Data, strlen((char*)rfm98Data));
	SPIWrite(LR_RegOpMode,0x8b);                    //Tx Mode
	//while(1)
//	{
		if(Get_NIRQ())   //Packet send over


		{RED_LED_L();
			SPIRead(LR_RegIrqFlags);

			rfm98_LoRaClearIrq();                                //Clear irq

			rfm98_Standby();                                     //Entry Standby mode

			//break;
		}
	//}
	return 0;
}

/**********************************************************
**Name:     rfm98_ReadRSSI
**Function: Read the RSSI value
**Input:    none
**Output:   temp, RSSI value
**********************************************************/
u8 rfm98_ReadRSSI(void)
{
  u8 temp=0xff;

  temp=SPIRead(0x11);
  temp>>=1;
  temp=127-temp;                                           //127:Max RSSI
  return temp;
}
/**********************************************************
**Name:     rfm98_Config
**Function: rfm98 base config
**Input:    mode
**Output:   None
**********************************************************/
void rfm98_Config(void)
{
  //u8 i;

  rfm98_Sleep();                                      //Change modem mode Must in Sleep mode
    //NOP();
	HAL_Delay(10);

  //lora mode
	rfm98_EntryLoRa();
	//SPIWrite(0x5904);   //?? Change digital regulator form 1.6V to 1.47V: see errata note

	BurstWrite(LR_RegFrMsb,rfm98FreqTbl[Freq_Sel],3);  //setting frequency parameter

	//setting base parameter
	SPIWrite(LR_RegPaConfig,rfm98PowerTbl[Power_Sel]);             //Setting output power parameter

	SPIWrite(LR_RegOcp,0x0B);                              //RegOcp,Close Ocp
	SPIWrite(LR_RegLna,0x23);                              //RegLNA,High & LNA Enable

	if(rfm98SpreadFactorTbl[Lora_Rate_Sel]==6)           //SFactor=6
	{
		u8 tmp;
		SPIWrite(LR_RegModemConfig1,((rfm98LoRaBwTbl[BandWide_Sel]<<4)+(CR<<1)+0x01));//Implicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
		SPIWrite(LR_RegModemConfig2,((rfm98SpreadFactorTbl[Lora_Rate_Sel]<<4)+(SPI_CRC<<2)+0x03));

		tmp = SPIRead(0x31);
		tmp &= 0xF8;
		tmp |= 0x05;
		SPIWrite(0x31,tmp);
		SPIWrite(0x37,0x0C);
	}
	else
	{
		SPIWrite(LR_RegModemConfig1,((rfm98LoRaBwTbl[BandWide_Sel]<<4)+(CR<<1)+0x00));//Explicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
		SPIWrite(LR_RegModemConfig2,((rfm98SpreadFactorTbl[Lora_Rate_Sel]<<4)+(SPI_CRC<<2)+0x03));  //SFactor &  LNA gain set by the internal AGC loop
	}
	SPIWrite(LR_RegSymbTimeoutLsb,0xFF);                   //RegSymbTimeoutLsb Timeout = 0x3FF(Max)

	SPIWrite(LR_RegPreambleMsb,0x00);                       //RegPreambleMsb
	SPIWrite(LR_RegPreambleLsb,12);                      //RegPreambleLsb 8+4=12byte Preamble

	SPIWrite(REG_LR_DIOMAPPING2,0x01);                     //RegDioMapping2 DIO5=00, DIO4=01

  rfm98_Standby();                                         //Entry standby mode
}

