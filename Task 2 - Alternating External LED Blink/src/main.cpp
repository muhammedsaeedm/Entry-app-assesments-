#include <Arduino.h>

#define EXT_LED1_PORT         GPIOA
#define EXT_LED1_PIN          GPIO_PIN_6

#define EXT_LED2_PORT         GPIOA
#define EXT_LED2_PIN          GPIO_PIN_7

#define BLINK_INTERVAL_MS     250

static void Task2_AlternatingLEDs(void);

void setup(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(EXT_LED1_PORT, EXT_LED1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EXT_LED2_PORT, EXT_LED2_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = EXT_LED1_PIN | EXT_LED2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(EXT_LED1_PORT, &GPIO_InitStruct);
}

void loop(void) {
    Task2_AlternatingLEDs();
}

static void Task2_AlternatingLEDs(void) {
    static uint32_t last_blink_time = 0;

    if ((millis() - last_blink_time) >= BLINK_INTERVAL_MS) {
        last_blink_time = millis();

        HAL_GPIO_TogglePin(EXT_LED1_PORT, EXT_LED1_PIN);
        HAL_GPIO_TogglePin(EXT_LED2_PORT, EXT_LED2_PIN);
    }
}
