// DS18B20.h
#ifndef DS18B20_H
#define DS18B20_H

#include "stm32f1xx_hal.h"

#define DS18B20_PORT ds18b_GPIO_Port
#define DS18B20_PIN  ds18b_Pin

void DS18B20_Init(void);
uint8_t DS18B20_Reset(void);
void DS18B20_WriteBit(uint8_t bit);
uint8_t DS18B20_ReadBit(void);
void DS18B20_WriteByte(uint8_t data);
uint8_t DS18B20_ReadByte(void);
float DS18B20_GetTemperature(void);
void DWT_Delay_us(uint32_t us);
void DWT_Delay_Init(void);

#endif /* DS18B20_H */

// DS18B20.c
