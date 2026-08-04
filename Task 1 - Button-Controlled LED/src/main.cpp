#include <Arduino.h>

/* Hardware Pin Definitions for STM32 Nucleo-F401RE */
#define USER_LED_PORT       GPIOA
#define USER_LED_PIN        GPIO_PIN_5

#define USER_BUTTON_PORT    GPIOC
#define USER_BUTTON_PIN     GPIO_PIN_13

#define DEBOUNCE_DELAY_MS   50

static void Task1_ButtonControlledLED(void);

void setup(void) {
    // Enable GPIO Clocks using HAL Driver APIs
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Configure User LED Pin (PA5) as Output
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_WritePin(USER_LED_PORT, USER_LED_PIN, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = USER_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(USER_LED_PORT, &GPIO_InitStruct);

    // Configure User Button Pin (PC13) as Input with Pull-Up
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
    static GPIO_PinState last_button_state = GPIO_PIN_SET; // Active LOW
    static GPIO_PinState button_state = GPIO_PIN_SET;

    // Read raw button state using Driver API (HAL_GPIO_ReadPin)
    GPIO_PinState current_read = HAL_GPIO_ReadPin(USER_BUTTON_PORT, USER_BUTTON_PIN);

    // Filter bouncing noise
    if (current_read != last_button_state) {
        last_debounce_time = millis();
    }

    if ((millis() - last_debounce_time) > DEBOUNCE_DELAY_MS) {
        if (current_read != button_state) {
            button_state = current_read;

            // Trigger on Falling Edge (Button Press: active LOW)
            if (button_state == GPIO_PIN_RESET) {
                // Toggle LED using Driver API (HAL_GPIO_TogglePin)
                HAL_GPIO_TogglePin(USER_LED_PORT, USER_LED_PIN);
            }
        }
    }

    last_button_state = current_read;
}
