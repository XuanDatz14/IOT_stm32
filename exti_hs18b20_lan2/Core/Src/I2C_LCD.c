#include "I2C_LCD.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1; // Sử dụng I2C1

void LCD_SendCommand(char cmd)
{
    char data_u = (cmd & 0xF0);
    char data_l = ((cmd << 4) & 0xF0);
    char data_t[4] = {data_u | 0x0C, data_u | 0x08, data_l | 0x0C, data_l | 0x08};
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDRESS, (uint8_t *)data_t, 4, 100);
}

void LCD_SendData(char data)
{
    char data_u = (data & 0xF0);
    char data_l = ((data << 4) & 0xF0);
    char data_t[4] = {data_u | 0x0D, data_u | 0x09, data_l | 0x0D, data_l | 0x09};
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDRESS, (uint8_t *)data_t, 4, 100);
}

void LCD_Clear(void)
{
    LCD_SendCommand(0x01);
    HAL_Delay(2);
}

void LCD_PutCursor(int row, int col)
{
    int pos = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_SendCommand(pos);
}

void LCD_SendString(char *str)
{
    while (*str)
        LCD_SendData(*str++);
}

void LCD_Init(void)
{
    HAL_Delay(50);
    LCD_SendCommand(0x30);
    HAL_Delay(5);
    LCD_SendCommand(0x30);
    HAL_Delay(1);
    LCD_SendCommand(0x30);
    HAL_Delay(10);
    LCD_SendCommand(0x20);
    HAL_Delay(10);

    LCD_SendCommand(0x28);
    LCD_SendCommand(0x08);
    LCD_SendCommand(0x01);
    HAL_Delay(2);
    LCD_SendCommand(0x06);
    LCD_SendCommand(0x0C);
}
