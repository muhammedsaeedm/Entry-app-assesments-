#include <Arduino.h>

#define USER_LED_PORT       GPIOA
#define USER_LED_PIN        GPIO_PIN_5

#define USER_BUTTON_PORT    GPIOC
#define USER_BUTTON_PIN     GPIO_PIN_13

#define DEBOUNCE_DELAY_MS   50

static void Task1_ButtonControlledLED(void);

void setup(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(USER_LED_PORT, USER_LED_PIN, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = USER_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(USER_LED_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USER_BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(USER_BUTTON_PORT, &GPIO_InitStruct);
}

void loop(void) {
    Task1_ButtonControlledLED();
}

static void Task1_ButtonControlledLED(void) {
    static uint32_t last_debounce_time = 0;
    static GPIO_PinState last_button_state = GPIO_PIN_SET;
    static GPIO_PinState button_state = GPIO_PIN_SET;

    GPIO_PinState current_read = HAL_GPIO_ReadPin(USER_BUTTON_PORT, USER_BUTTON_PIN);

    if (current_read != last_button_state) {
        last_debounce_time = millis();
    }

    if ((millis() - last_debounce_time) > DEBOUNCE_DELAY_MS) {
        if (current_read != button_state) {
            button_state = current_read;

            if (button_state == GPIO_PIN_RESET) {
                HAL_GPIO_TogglePin(USER_LED_PORT, USER_LED_PIN);
            }
        }
    }

    last_button_state = current_read;
}
