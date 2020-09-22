/*
 * tmp102.h
 *
 *  Created on: Feb 14, 2020
 *      Author: G K Praful
 */

#ifndef INC_TMP102_H_
#define INC_TMP102_H_
#include "my.h"

#define TMP102_ADDR   0x48<<1
#define REG_TEMP      0x00

extern u8 tmp102(void);

#endif /* INC_TMP102_H_ */
