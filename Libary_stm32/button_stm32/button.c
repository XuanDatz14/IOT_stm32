#include "button.h"
#include "stm32f1xx_hal.h"

// Khởi tạo nút
void Button_Init(Button_t *btn, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    btn->GPIOx = GPIOx;
    btn->GPIO_Pin = GPIO_Pin;
    btn->state = BUTTON_IDLE;
    btn->lastDebounceTime = HAL_GetTick();
    btn->pressStartTime = 0;
    btn->callbackPressed = NULL;
    btn->callbackReleased = NULL;
    btn->callbackHold = NULL;
}

// Cập nhật trạng thái nút, nên gọi trong vòng lặp `while(1)`
void Button_Update(Button_t *btn) {
    uint8_t buttonRead = HAL_GPIO_ReadPin(btn->GPIOx, btn->GPIO_Pin);
    uint32_t now = HAL_GetTick();

    if (buttonRead == GPIO_PIN_RESET) { // Nút được nhấn
        if (btn->state == BUTTON_IDLE) {
            if (now - btn->lastDebounceTime > DEBOUNCE_TIME) {
                btn->state = BUTTON_PRESSED;
                btn->pressStartTime = now;
                if (btn->callbackPressed) btn->callbackPressed();
            }
        } else if (btn->state == BUTTON_PRESSED) {
            if (now - btn->pressStartTime > HOLD_TIME) {
                btn->state = BUTTON_HOLD;
                if (btn->callbackHold) btn->callbackHold();
            }
        }
    } else { // Nút được thả ra
        if (btn->state == BUTTON_PRESSED || btn->state == BUTTON_HOLD) {
            if (now - btn->lastDebounceTime > DEBOUNCE_TIME) {
                btn->state = BUTTON_RELEASED;
                if (btn->callbackReleased) btn->callbackReleased();
            }
        }
        btn->state = BUTTON_IDLE;
    }

    btn->lastDebounceTime = now;
}

// Thiết lập callback khi nhấn nút
void Button_SetPressedCallback(Button_t *btn, void (*callback)(void)) {
    btn->callbackPressed = callback;
}

// Thiết lập callback khi thả nút
void Button_SetReleasedCallback(Button_t *btn, void (*callback)(void)) {
    btn->callbackReleased = callback;
}

// Thiết lập callback khi giữ nút
void Button_SetHoldCallback(Button_t *btn, void (*callback)(void)) {
    btn->callbackHold = callback;
}
