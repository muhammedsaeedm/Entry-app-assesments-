# Integrated Assessment Tasks: STM32 Nucleo-F401RE Board

This project integrates **Task 1 (Button-Controlled Onboard LED)** and **Task 2 (Alternating External LED Blink)** using strictly **GPIO Driver APIs** (no direct register manipulation).

---

## 1. Pin Mappings

| Hardware Component | STM32 GPIO Port & Pin | Nucleo Header Location | Configuration Mode |
| :--- | :--- | :--- | :--- |
| **Onboard User LED (LD2)** | `GPIOA`, `GPIO_PIN_5` | Internal | Push-Pull Output |
| **Onboard User Button (B1)** | `GPIOC`, `GPIO_PIN_13` | Internal | Input with Pull-Up (Active LOW) |
| **External LED 1** | `GPIOA`, `GPIO_PIN_6` | Arduino Header `D12` / Morpho `PA6` | Push-Pull Output |
| **External LED 2** | `GPIOA`, `GPIO_PIN_7` | Arduino Header `D11` / Morpho `PA7` | Push-Pull Output |

---

## 2. Task 2 Wiring Diagram

Connect two external LEDs with current-limiting resistors (220Ω - 470Ω) to `PA6` and `PA7` on the Nucleo board:

```
                  STM32 Nucleo-F401RE Board
             +---------------------------------+
             |                                 |
             |   [PA6 / D12] ----[ 220Ω ]--->|---(GND)
             |                               Ext LED 1
             |                                 |
             |   [PA7 / D11] ----[ 220Ω ]--->|---(GND)
             |                               Ext LED 2
             |                                 |
             |   [GND Pin]   ------------------+
             +---------------------------------+
```

---

## 3. Implementation Details & Driver APIs

1. **Task 1: Button-Controlled LED**:
   - Reads `PC13` using `HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)`.
   - Filters mechanical contact bounce with a 50ms software debounce window.
   - Detects the falling edge (button press) and toggles `PA5` via `HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5)`.
   - LED state persists reliably after button release.

2. **Task 2: Alternating External LED Blink**:
   - Initialized with `PA6` set `HIGH` and `PA7` set `LOW`.
   - Alternates state every 250ms (yielding a 500ms total period per LED) using `HAL_GPIO_TogglePin()`.
   - Runs out-of-phase smoothly in the main loop without blocking Task 1 button events.

3. **Driver Function Rule Compliance**:
   - All GPIO operations use official driver functions (`HAL_GPIO_Init`, `HAL_GPIO_ReadPin`, `HAL_GPIO_WritePin`, `HAL_GPIO_TogglePin`).
   - Zero direct register access (`GPIOA->ODR` or `GPIOC->IDR` avoided completely).

---

## 4. Video Demonstration & Verification

- **Video Demo File**: [task1_demo.mp4](task1_demo.mp4)
- **Observations**:
  - Button presses on B1 cleanly toggle LD2 (PA5) on demand.
  - External LEDs on PA6 and PA7 alternate cleanly out of phase every 250ms.
