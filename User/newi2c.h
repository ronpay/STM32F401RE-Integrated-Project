#ifndef _NEWI2C_H
#define _NEWI2C_H

#include"stm32f4xx.h"

void I2C_WriteByte(I2C_TypeDef * I2Cx,uint8_t slave_addr,uint8_t reg_addr, uint8_t data);
void I2C_ReadData(I2C_TypeDef * I2Cx,uint8_t slave_addr, uint8_t reg_addr, uint8_t *pBuffer,uint16_t NumByteToRead);


#endif
