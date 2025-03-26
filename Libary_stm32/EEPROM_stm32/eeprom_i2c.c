/* eeprom_i2c.c */
#include "eeprom_i2c.h"

void EEPROM_WriteByte(uint16_t memAddr, uint8_t data)
{
    uint8_t temp[2] = {(uint8_t)(memAddr & 0xFF), data};
    HAL_I2C_Master_Transmit(&hi2c1, EEPROM_I2C_ADDRESS, temp, 2, 100);
    HAL_Delay(5);
}

uint8_t EEPROM_ReadByte(uint16_t memAddr)
{
    uint8_t data;
    uint8_t addr = (uint8_t)(memAddr & 0xFF);
    HAL_I2C_Master_Transmit(&hi2c1, EEPROM_I2C_ADDRESS, &addr, 1, 100);
    HAL_I2C_Master_Receive(&hi2c1, EEPROM_I2C_ADDRESS, &data, 1, 100);
    return data;
}

void EEPROM_WritePage(uint16_t memAddr, uint8_t *data, uint16_t len)
{
    uint8_t buffer[len + 1];
    buffer[0] = (uint8_t)(memAddr & 0xFF);
    memcpy(&buffer[1], data, len);
    HAL_I2C_Master_Transmit(&hi2c1, EEPROM_I2C_ADDRESS, buffer, len + 1, 100);
    HAL_Delay(5);
}

void EEPROM_ReadBuffer(uint16_t memAddr, uint8_t *buffer, uint16_t len)
{
    uint8_t addr = (uint8_t)(memAddr & 0xFF);
    HAL_I2C_Master_Transmit(&hi2c1, EEPROM_I2C_ADDRESS, &addr, 1, 100);
    HAL_I2C_Master_Receive(&hi2c1, EEPROM_I2C_ADDRESS, buffer, len, 100);
}
