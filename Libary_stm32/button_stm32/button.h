#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f1xx_hal.h"

#define DEBOUNCE_TIME 50   // Thời gian chống dội phím (ms)
#define HOLD_TIME 1000     // Thời gian giữ để nhận sự kiện (ms)

typedef enum {
    BUTTON_IDLE = 0,
    BUTTON_PRESSED,
    BUTTON_RELEASED,
    BUTTON_HOLD
} ButtonState;

typedef struct {
    GPIO_TypeDef *GPIOx;  // Cổng GPIO
    uint16_t GPIO_Pin;    // Chân GPIO
    ButtonState state;    // Trạng thái nút
    uint32_t lastDebounceTime;
    uint32_t pressStartTime;
    void (*callbackPressed)(void);
    void (*callbackReleased)(void);
    void (*callbackHold)(void);
} Button_t;

void Button_Init(Button_t *btn, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Button_Update(Button_t *btn);
void Button_SetPressedCallback(Button_t *btn, void (*callback)(void));
void Button_SetReleasedCallback(Button_t *btn, void (*callback)(void));
void Button_SetHoldCallback(Button_t *btn, void (*callback)(void));

#endif
