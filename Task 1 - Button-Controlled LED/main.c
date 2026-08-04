/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Integrated Task 1 & Task 2 for STM32 Nucleo-F401RE Board
  * @details        : Task 1: Onboard Button (PC13) toggles Onboard LED (PA5).
  *                   Task 2: Alternating blink of External LEDs on PA6 & PA7
  *                   with 250ms state changes (500ms total period).
  *                   Uses strictly Driver APIs for all GPIO operations.
  ******************************************************************************
  */

#include "main.h"

/* -------------------------------------------------------------------------- */
/* Hardware Pin Definitions (Verified against STM32 Nucleo-F401RE Schematic)  */
/* -------------------------------------------------------------------------- */

/* Onboard Hardware Pins */
#define ONBOARD_LED_PORT      GPIOA
#define ONBOARD_LED_PIN       GPIO_PIN_5    /* LD2 Onboard Green LED */

#define ONBOARD_BUTTON_PORT   GPIOC
#define ONBOARD_BUTTON_PIN    GPIO_PIN_13   /* B1 Onboard User Button (Active LOW) */

/* External Hardware Pins for Task 2 */
#define EXT_LED1_PORT         GPIOA
#define EXT_LED1_PIN          GPIO_PIN_6    /* External LED 1 (Morpho Pin PA6 / Arduino D12) */

#define EXT_LED2_PORT         GPIOA
#define EXT_LED2_PIN          GPIO_PIN_7    /* External LED 2 (Morpho Pin PA7 / Arduino D11) */

/* Timing Parameters */
#define DEBOUNCE_DELAY_MS     50    /* 50 ms debounce window for button */
#define BLINK_INTERVAL_MS     250   /* 250 ms per state (250ms ON, 250ms OFF = 500ms period) */

/* Function Prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Task1_ButtonControlledLED(void);
void Task2_AlternatingLEDs(void);
void delay_ms(uint32_t ms);

/**
  * @brief  Main entry point integrating Task 1 and Task 2.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes Flash interface and Systick */
  HAL_Init();

  /* Configure System Clock */
  SystemClock_Config();

  /* Initialize GPIO Clocks and Pins using Driver Functions */
  MX_GPIO_Init();

  /* Set initial state for Task 2 alternating LEDs (LED 1 ON, LED 2 OFF) */
  HAL_GPIO_WritePin(EXT_LED1_PORT, EXT_LED1_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(EXT_LED2_PORT, EXT_LED2_PIN, GPIO_PIN_RESET);

  /* Infinite Main Loop */
  while (1)
  {
    /* Task 1: Poll button with non-blocking software debounce to toggle LD2 */
    Task1_ButtonControlledLED();

    /* Task 2: Manage alternating external LED blink pattern */
    Task2_AlternatingLEDs();
  }
}

/**
  * @brief  Task 1 Implementation: Toggles Onboard LED (PA5) on Button (PC13) press.
  * @note   Uses HAL_GPIO_ReadPin and HAL_GPIO_TogglePin (Strictly Driver APIs).
  *         Implements non-blocking debouncing so Task 2 timing remains smooth.
  */
void Task1_ButtonControlledLED(void)
{
    static uint32_t last_debounce_time = 0;
    static GPIO_PinState last_button_state = GPIO_PIN_SET; // Button is active LOW
    static GPIO_PinState button_state = GPIO_PIN_SET;

    // Read button pin using Driver API
    GPIO_PinState current_read = HAL_GPIO_ReadPin(ONBOARD_BUTTON_PORT, ONBOARD_BUTTON_PIN);

    // Reset debounce timer on signal edge change
    if (current_read != last_button_state) {
        last_debounce_time = HAL_GetTick();
    }

    // Filter bouncing noise
    if ((HAL_GetTick() - last_debounce_time) > DEBOUNCE_DELAY_MS) {
        if (current_read != button_state) {
            button_state = current_read;

            // Trigger on Falling Edge (Physical Button Press)
            if (button_state == GPIO_PIN_RESET) {
                // Toggle onboard LED state using Driver API
                HAL_GPIO_TogglePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN);
            }
        }
    }

    last_button_state = current_read;
}

/**
  * @brief  Task 2 Implementation: Alternates External LEDs (PA6 and PA7) out-of-phase.
  * @note   Each LED is ON for 250ms and OFF for 250ms (Total period = 500ms).
  *         Uses HAL_GPIO_TogglePin for reliable out-of-phase operation.
  */
void Task2_AlternatingLEDs(void)
{
    static uint32_t last_blink_time = 0;

    // Check if 250ms interval has elapsed
    if ((HAL_GetTick() - last_blink_time) >= BLINK_INTERVAL_MS) {
        last_blink_time = HAL_GetTick();

        // Toggle both pins simultaneously using Driver APIs
        // Since LED1 started HIGH and LED2 started LOW, toggling preserves inverse state
        HAL_GPIO_TogglePin(EXT_LED1_PORT, EXT_LED1_PIN);
        HAL_GPIO_TogglePin(EXT_LED2_PORT, EXT_LED2_PIN);
    }
}

/**
  * @brief  Simple busy-wait delay function as per requirements.
  * @param  ms Delay time in milliseconds
  */
void delay_ms(uint32_t ms)
{
    uint32_t start = HAL_GetTick();
    while ((HAL_GetTick() - start) < ms) {
        /* Busy wait loop */
    }
}

/**
  * @brief  GPIO Peripheral Initialization
  * @details Configures PA5, PA6, PA7 as Push-Pull Outputs and PC13 as Input with Pull-Up.
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable Clocks for GPIOA and GPIOC peripherals */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* -------------------------------------------------------------------- */
  /* Configure Onboard LED (PA5) as Output                                */
  /* -------------------------------------------------------------------- */
  HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = ONBOARD_LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ONBOARD_LED_PORT, &GPIO_InitStruct);

  /* -------------------------------------------------------------------- */
  /* Configure External LEDs for Task 2 (PA6 & PA7) as Outputs            */
  /* -------------------------------------------------------------------- */
  HAL_GPIO_WritePin(EXT_LED1_PORT, EXT_LED1_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EXT_LED2_PORT, EXT_LED2_PIN, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = EXT_LED1_PIN | EXT_LED2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(EXT_LED1_PORT, &GPIO_InitStruct);

  /* -------------------------------------------------------------------- */
  /* Configure Onboard User Button (PC13) as Input with Internal Pull-Up */
  /* -------------------------------------------------------------------- */
  GPIO_InitStruct.Pin = ONBOARD_BUTTON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ONBOARD_BUTTON_PORT, &GPIO_InitStruct);
}

/**
  * @brief System Clock Configuration stub
  */
void SystemClock_Config(void)
{
}
