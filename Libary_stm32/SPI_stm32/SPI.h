#ifndef SPI_H
#define SPI_H

#include "stm32f1xx_hal.h"

void SPI_INIT(SPI_HandleTypeDef *hspi);
uint8_t SPI_TRANSMIT_RECEIVE(uint8_t data);
void SPI_SELECT(void);
void SPI_DESELECT(void);

#endif
