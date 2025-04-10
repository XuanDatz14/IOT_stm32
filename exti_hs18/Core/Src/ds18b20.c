#include "DS18B20.h"
#include "main.h"

void DWT_Delay_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void DWT_Delay_us(uint32_t us) {
    uint32_t startTick = DWT->CYCCNT;
    uint32_t delayTicks = us * (SystemCoreClock / 1000000);
    while ((DWT->CYCCNT - startTick) < delayTicks);
}

void DS18B20_Pin_Output(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = ds18_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ds18_GPIO_Port, &GPIO_InitStruct);
}

void DS18B20_Pin_Input(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = ds18_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ds18_GPIO_Port, &GPIO_InitStruct);
}

uint8_t DS18B20_Reset(void) {
    DS18B20_Pin_Output();
    HAL_GPIO_WritePin(ds18_GPIO_Port, ds18_Pin, GPIO_PIN_RESET);
    DWT_Delay_us(480);
    DS18B20_Pin_Input();
    DWT_Delay_us(70);
    uint8_t presence = HAL_GPIO_ReadPin(ds18_GPIO_Port, ds18_Pin);
    DWT_Delay_us(410);
    return presence == 0 ? 1 : 0;
}

void DS18B20_WriteBit(uint8_t bit) {
    DS18B20_Pin_Output();
    HAL_GPIO_WritePin(ds18_GPIO_Port, ds18_Pin, GPIO_PIN_RESET);
    DWT_Delay_us(2);
    if (bit) HAL_GPIO_WritePin(ds18_GPIO_Port, ds18_Pin, GPIO_PIN_SET);
    DWT_Delay_us(60);
    DS18B20_Pin_Input();
}

uint8_t DS18B20_ReadBit(void) {
    DS18B20_Pin_Output();
    HAL_GPIO_WritePin(ds18_GPIO_Port, ds18_Pin, GPIO_PIN_RESET);
    DWT_Delay_us(2);
    DS18B20_Pin_Input();
    DWT_Delay_us(10);
    uint8_t bit = HAL_GPIO_ReadPin(ds18_GPIO_Port, ds18_Pin);
    DWT_Delay_us(50);
    return bit;
}

void DS18B20_WriteByte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        DS18B20_WriteBit(data & 0x01);
        data >>= 1;
    }
}

uint8_t DS18B20_ReadByte(void) {
    uint8_t value = 0;
    for (uint8_t i = 0; i < 8; i++) {
        value >>= 1;
        if (DS18B20_ReadBit()) value |= 0x80;
    }
    return value;
}

float DS18B20_GetTemperature(void) {
    if (!DS18B20_Reset()) return -1000;
    DS18B20_WriteByte(0xCC);
    DS18B20_WriteByte(0x44);
    while (!DS18B20_ReadBit());
    if (!DS18B20_Reset()) return -1000;
    DS18B20_WriteByte(0xCC);
    DS18B20_WriteByte(0xBE);
    uint8_t temp_LSB = DS18B20_ReadByte();
    uint8_t temp_MSB = DS18B20_ReadByte();
    int16_t temp = (temp_MSB << 8) | temp_LSB;
    return temp / 16.0;
}
