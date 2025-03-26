// DS18B20.h
#ifndef DS18B20_H
#define DS18B20_H

#include "stm32f1xx_hal.h"

// Cấu hình GPIO cho DS18B20
#define DS18B20_GPIO_Port GPIOA
#define DS18B20_Pin GPIO_PIN_8

// Khởi tạo DWT Delay
void DWT_Delay_Init(void);
void DWT_Delay_us(uint32_t us);

// Cấu hình chân GPIO
void DS18B20_Pin_Output(void);
void DS18B20_Pin_Input(void);

// Giao tiếp 1-Wire
uint8_t DS18B20_Reset(void);
void DS18B20_WriteBit(uint8_t bit);
uint8_t DS18B20_ReadBit(void);
void DS18B20_WriteByte(uint8_t data);
uint8_t DS18B20_ReadByte(void);

// Đọc nhiệt độ
float DS18B20_GetTemperature(void);

#endif