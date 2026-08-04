#include "main.h"

#define ONBOARD_LED_PORT      GPIOA
#define ONBOARD_LED_PIN       GPIO_PIN_5

#define ONBOARD_BUTTON_PORT   GPIOC
#define ONBOARD_BUTTON_PIN    GPIO_PIN_13

#define EXT_LED1_PORT         GPIOA
#define EXT_LED1_PIN          GPIO_PIN_6

#define EXT_LED2_PORT         GPIOA
#define EXT_LED2_PIN          GPIO_PIN_7

#define DEBOUNCE_DELAY_MS     50
#define BLINK_INTERVAL_MS     250

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Task1_ButtonControlledLED(void);
void Task2_AlternatingLEDs(void);
void delay_ms(uint32_t ms);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  HAL_GPIO_WritePin(EXT_LED1_PORT, EXT_LED1_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EXT_LED2_PORT, EXT_LED2_PIN, GPIO_PIN_RESET);

  while (1)
  {
    Task1_ButtonControlledLED();
    Task2_AlternatingLEDs();
  }
}

void Task1_ButtonControlledLED(void)
{
    static uint32_t last_debounce_time = 0;
    static GPIO_PinState last_button_state = GPIO_PIN_SET;
    static GPIO_PinState button_state = GPIO_PIN_SET;

    GPIO_PinState current_read = HAL_GPIO_ReadPin(ONBOARD_BUTTON_PORT, ONBOARD_BUTTON_PIN);

    if (current_read != last_button_state) {
        last_debounce_time = HAL_GetTick();
    }

    if ((HAL_GetTick() - last_debounce_time) > DEBOUNCE_DELAY_MS) {
        if (current_read != button_state) {
            button_state = current_read;

            if (button_state == GPIO_PIN_RESET) {
                HAL_GPIO_TogglePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN);
            }
        }
    }

    last_button_state = current_read;
}

void Task2_AlternatingLEDs(void)
{
    static uint32_t last_blink_time = 0;

    if ((HAL_GetTick() - last_blink_time) >= BLINK_INTERVAL_MS) {
        last_blink_time = HAL_GetTick();

        HAL_GPIO_TogglePin(EXT_LED1_PORT, EXT_LED1_PIN);
        HAL_GPIO_TogglePin(EXT_LED2_PORT, EXT_LED2_PIN);
    }
}

void delay_ms(uint32_t ms)
{
    uint32_t start = HAL_GetTick();
    while ((HAL_GetTick() - start) < ms) {
    }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = ONBOARD_LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ONBOARD_LED_PORT, &GPIO_InitStruct);

  HAL_GPIO_WritePin(EXT_LED1_PORT, EXT_LED1_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EXT_LED2_PORT, EXT_LED2_PIN, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = EXT_LED1_PIN | EXT_LED2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EXT_LED1_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ONBOARD_BUTTON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ONBOARD_BUTTON_PORT, &GPIO_InitStruct);
}

void SystemClock_Config(void)
{
}
