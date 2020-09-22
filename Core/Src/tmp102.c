/*
 * tmp102.c
 *
 *  Created on: Feb 14, 2020
 *      Author: G K Praful
 */

#include "tmp102.h"
#include "string.h"
#include "main.h"
#include "my.h"


//static const uint8_t TMP102_ADDR = 0x48<<1;
//static const uint8_t REG_TEMP = 0x00;
I2C_HandleTypeDef hi2c1;

u8 tmp102(void)
{
	HAL_StatusTypeDef ret;
	uint8_t buf[12];
	int16_t val;
	float temp_c;
		 buf[0] = REG_TEMP;
		 ret = HAL_I2C_Master_Transmit(&hi2c1, TMP102_ADDR, buf, 1,HAL_MAX_DELAY);
		 if ( ret != HAL_OK)
		 {
		 	 strcpy ((char*)buf,"Error Tx\r\n");
		 }
		 else
		 {
			 ret = HAL_I2C_Master_Receive(&hi2c1, TMP102_ADDR, buf, 2,HAL_MAX_DELAY);
			 if ( ret != HAL_OK){
			  		 strcpy ((char*)buf,"Error Rx\r\n");
			  	  }
			 else{
				 //Combine the bytes
			  		  val = ((int16_t)buf[0] << 4) | (buf[1] >> 4);
			  		  // Convert to 2's complement, since temperature can be negative
			  		   if ( val > 0x7FF ) {
			  		          val |= 0xF000;
			  		        }

			  		        // Convert to float temperature value (Celsius)
			  		        temp_c = val * 0.0625;

			  		        // Convert temperature to decimal format
			  		        temp_c *= 100;
			  		        sprintf((char*)buf, "%u.%u C\r\n",
			  		              ((unsigned int)temp_c / 100),
			  		              ((unsigned int)temp_c % 100));
			 	 }
		 }
			  return (buf);
}
