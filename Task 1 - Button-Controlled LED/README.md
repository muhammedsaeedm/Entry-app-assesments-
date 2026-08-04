# Task 1: Button-Controlled LED (STM32 Nucleo Board)

## Overview
This task implements a reliable button-controlled LED toggle on an STM32 Nucleo board using **only GPIO driver APIs** (no direct register access).

## Hardware Configuration
- **Onboard User LED (LD2)**: `GPIOA`, `GPIO_PIN_5` (Push-Pull Output)
- **Onboard User Button (B1)**: `GPIOC`, `GPIO_PIN_13` (Input with internal Pull-Up, Active LOW)

## Video Demo
The video demonstration showing hardware behavior and single-press LED toggling is included in this directory:
- [Task 1 Video Demo](task1_demo.mp4)

## Key Features & Constraints
1. **Strict API Usage**:
   - `HAL_GPIO_ReadPin()` to read the button state.
   - `HAL_GPIO_TogglePin()` / `HAL_GPIO_WritePin()` to control the LED state.
   - `HAL_GPIO_Init()` to initialize GPIO pins.
   - No direct register writes (e.g. `GPIOA->ODR` or `GPIOC->IDR`).

2. **Software Debouncing & State Persistence**:
   - Uses a non-blocking 50ms software debounce algorithm using `millis()` / `HAL_GetTick()`.
   - Detects the falling edge (button press) to toggle the LED state exactly once per press.
   - LED state persists after button release.

## Explanation of Changes
- **Debounce Logic**: Physical buttons suffer from mechanical contact bounce (10ms–30ms). A state timer (`last_debounce_time`) filters high-frequency noise before validating a genuine state change.
- **Edge Detection**: Toggling is performed only when transitioning from `SET` to `RESET` (`button_state == GPIO_PIN_RESET`), ensuring release events do not re-trigger the LED.
