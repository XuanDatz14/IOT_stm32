/* eeprom_i2c.h */
#ifndef EEPROM_I2C_H
#define EEPROM_I2C_H

#include "stm32f1xx_hal.h"

#define EEPROM_I2C_ADDRESS 0x50 << 1  // Địa chỉ mặc định của EEPROM 24Cxx

extern I2C_HandleTypeDef hi2c1;

void EEPROM_WriteByte(uint16_t memAddr, uint8_t data);
uint8_t EEPROM_ReadByte(uint16_t memAddr);
void EEPROM_WritePage(uint16_t memAddr, uint8_t *data, uint16_t len);
void EEPROM_ReadBuffer(uint16_t memAddr, uint8_t *buffer, uint16_t len);

#endif /* EEPROM_I2C_H */

