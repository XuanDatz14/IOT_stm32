#ifndef I2C_LCD_H
#define I2C_LCD_H

#include "stm32f1xx_hal.h"

#define LCD_I2C_ADDRESS (0x27 << 1) // Địa chỉ I2C của LCD

void LCD_Init(void);
void LCD_SendCommand(char cmd);
void LCD_SendData(char data);
void LCD_SendString(char *str);
void LCD_PutCursor(int row, int col);
void LCD_Clear(void);

#endif
