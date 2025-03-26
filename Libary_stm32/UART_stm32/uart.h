#ifndef UART_H
#define UART_H

#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdio.h>

// Sử dụng USART1, có thể thay đổi thành USART2, USART3 nếu cần
extern UART_HandleTypeDef huart1;

// Gửi một chuỗi ký tự qua UART
void UART_SendString(char *string);

// Gửi một ký tự đơn qua UART
void UART_SendChar(char c);

// Gửi một số nguyên qua UART
void UART_SendInt(int number);

// Nhận một chuỗi ký tự qua UART
void UART_ReceiveString(char *buffer, uint16_t size);

// Hàm tương tự printf để in ra UART
void UART_Printf(const char *format, ...);

#endif // UART_H
