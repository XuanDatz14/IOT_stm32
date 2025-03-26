#include "C:\Libary_stm32\UART_stm32\uart.h"
#include <stdarg.h>  // Thư viện hỗ trợ truyền tham số không cố định

/**
 * @brief Gửi một chuỗi qua UART
 * @param string: Chuỗi cần gửi
 */
void UART_SendString(char *string) {
    HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 100);
}

/**
 * @brief Gửi một ký tự qua UART
 * @param c: Ký tự cần gửi
 */
void UART_SendChar(char c) {
    HAL_UART_Transmit(&huart1, (uint8_t*)&c, 1, 10);
}

/**
 * @brief Gửi một số nguyên dưới dạng chuỗi qua UART
 * @param number: Số nguyên cần gửi
 */
void UART_SendInt(int number) {
    char buffer[12];  // Bộ đệm đủ lớn để chứa số nguyên
    sprintf(buffer, "%d", number);  // Chuyển số thành chuỗi
    UART_SendString(buffer);
}

/**
 * @brief Nhận một chuỗi dữ liệu từ UART
 * @param buffer: Mảng chứa dữ liệu nhận được
 * @param size: Số lượng byte cần nhận
 */
void UART_ReceiveString(char *buffer, uint16_t size) {
    HAL_UART_Receive(&huart1, (uint8_t*)buffer, size, 1000);
    buffer[size] = '\0';  // Đảm bảo chuỗi kết thúc đúng
}

/**
 * @brief Hàm printf gửi dữ liệu qua UART
 * @param format: Chuỗi định dạng (giống printf)
 */
void UART_Printf(const char *format, ...) {
    char buffer[128];  // Bộ đệm chứa dữ liệu cần in
    va_list args;
    va_start(args, format);  // Bắt đầu lấy danh sách tham số không cố định
    vsnprintf(buffer, sizeof(buffer), format, args);  // Định dạng chuỗi
    va_end(args);
    UART_SendString(buffer);  // Gửi chuỗi qua UART
}
