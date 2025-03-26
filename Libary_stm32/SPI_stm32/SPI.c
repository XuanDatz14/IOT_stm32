#include "SPI.h"

// Khai báo SPI_HandleTypeDef bên ngoài để dùng trong thư viện
extern SPI_HandleTypeDef hspi1;

// Hàm khởi tạo SPI
void SPI_INIT(SPI_HandleTypeDef *hspi)
{
    hspi->Instance = SPI1;
    hspi->Init.Mode = SPI_MODE_MASTER;
    hspi->Init.Direction = SPI_DIRECTION_2LINES;
    hspi->Init.DataSize = SPI_DATASIZE_8BIT;
    hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi->Init.NSS = SPI_NSS_SOFT;
    hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi->Init.TIMode = SPI_TIMODE_DISABLE;
    hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    HAL_SPI_Init(hspi);
}

// Hàm truyền và nhận 1 byte qua SPI
uint8_t SPI_TRANSMIT_RECEIVE(uint8_t data)
{
    uint8_t receivedData = 0;
    HAL_SPI_TransmitReceive(&hspi1, &data, &receivedData, 1, HAL_MAX_DELAY);
    return receivedData;
}

// Hàm chọn slave (SS = 0)
void SPI_SELECT(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

// Hàm bỏ chọn slave (SS = 1)
void SPI_DESELECT(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
