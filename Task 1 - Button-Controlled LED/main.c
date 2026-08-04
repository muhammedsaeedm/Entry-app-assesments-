/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Task 1 - Button-Controlled LED for STM32 Nucleo Board
  * @details        : Toggles onboard User LED (PA5) on User Button (PC13) press
  *                   using only GPIO Driver APIs and software debouncing.
  ******************************************************************************
  */

#include "main.h"

/* Onboard Hardware Definitions for STM32 Nucleo Board */
#define USER_LED_PORT       GPIOA
#define USER_LED_PIN        GPIO_PIN_5

#define USER_BUTTON_PORT    GPIOC
#define USER_BUTTON_PIN     GPIO_PIN_13

#define DEBOUNCE_DELAY_MS   50   /* 50 ms debounce threshold */

/* Private Function Prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Task1_ButtonControlledLED(void);

/**
  * @brief  Main program entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals (GPIO) */
  MX_GPIO_Init();

  /* Infinite loop */
  while (1)
  {
    /* Execute Task 1: Button-Controlled LED with Debouncing */
    Task1_ButtonControlledLED();
  }
}

/**
  * @brief  Task 1 Implementation: Toggles User LED reliably on User Button press.
  * @note   Uses strictly GPIO Driver APIs (HAL_GPIO_ReadPin, HAL_GPIO_TogglePin).
  *         Implements non-blocking software debouncing via HAL_GetTick().
  * @retval None
  */
void Task1_ButtonControlledLED(void)
{
    static uint32_t last_debounce_time = 0;
    static GPIO_PinState last_button_state = GPIO_PIN_SET; // Button is active LOW (Pull-Up)
    static GPIO_PinState button_state = GPIO_PIN_SET;

    // 1. Read raw button state using Driver API (No direct register access)
    GPIO_PinState current_read = HAL_GPIO_ReadPin(USER_BUTTON_PORT, USER_BUTTON_PIN);

    // 2. If state changed (due to noise or press), reset debounce timer
    if (current_read != last_button_state) {
        last_debounce_time = HAL_GetTick();
    }

    // 3. If signal has been stable longer than DEBOUNCE_DELAY_MS
    if ((HAL_GetTick() - last_debounce_time) > DEBOUNCE_DELAY_MS) {
        // Check if button state has genuinely changed
        if (current_read != button_state) {
            button_state = current_read;

            // 4. Trigger state toggle ONLY on falling edge (Button Press: active LOW)
            if (button_state == GPIO_PIN_RESET) {
                // Toggle onboard LED state using Driver API
                HAL_GPIO_TogglePin(USER_LED_PORT, USER_LED_PIN);
            }
        }
    }

    // Save current reading for next iteration
    last_button_state = current_read;
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable GPIO Clocks using HAL Driver APIs */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* Configure User LED Pin (PA5) as Output */
  HAL_GPIO_WritePin(USER_LED_PORT, USER_LED_PIN, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = USER_LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USER_LED_PORT, &GPIO_InitStruct);

  /* Configure User Button Pin (PC13) as Input */
  GPIO_InitStruct.Pin = USER_BUTTON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(USER_BUTTON_PORT, &GPIO_InitStruct);
}

/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
  /* Clock configuration initialization per board specs */
}
