# STM32 Low Power — Sleep Mode

This project demonstrates how to enter and exit **Sleep Mode** on the STM32 using the HAL library in STM32CubeIDE.

In Sleep mode, the CPU clock is halted while all peripheral clocks continue to run. The MCU can wake up from any interrupt, making this the fastest and most responsive low-power mode.

This is part of the STM32 Low Power Modes series on [Controllerstech](https://controllerstech.com/low-power-modes-in-stm32/).

---

## Hardware

- **Board:** STM32 (e.g. STM32F446RE / STM32F103C8)
- **Wake-up sources used:**
  - UART2 RX interrupt
  - External interrupt (EXTI) via user button (GPIO pin)
- **LED:** PA5 — used to indicate sleep/wake state

---

## How Sleep Mode Works

| Property | Value |
|---|---|
| **CPU** | Stopped |
| **Peripherals** | Running |
| **SRAM / Registers** | Retained |
| **Current Draw (typical)** | ~2–5 mA |
| **Wake-up time** | Very fast |
| **Reset on wake-up** | No |

---

## Entering Sleep Mode

SysTick is suspended first to prevent it from waking the MCU every 1ms:

```c
HAL_SuspendTick();
HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
```

After wake-up, SysTick is resumed:

```c
HAL_ResumeTick();
```

---

## Wake-up Sources

### UART Interrupt
```c
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_UART_Receive_IT(huart, &Rx_data, 1);
    // SleepOnExit remains active — MCU goes back to sleep after this ISR
}
```

### External Interrupt (Button)
```c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    HAL_ResumeTick();
    HAL_PWR_DisableSleepOnExit();  // Return to main loop after this ISR
}
```

---

## SLEEPONEXIT Feature

`HAL_PWR_EnableSleepOnExit()` makes the MCU automatically re-enter Sleep mode after each ISR exits, without returning to the main loop. This is ideal for interrupt-driven, ultra-low-power applications.

- Woken by **UART** → ISR runs → goes back to sleep (SLEEPONEXIT active)
- Woken by **EXTI** → ISR runs → returns to main loop (SLEEPONEXIT disabled inside ISR)

---

## Project Setup

1. Clone or download this repository
2. Open **STM32CubeIDE**
3. Go to **File → Import → Existing Projects into Workspace**
4. Select this folder and click **Finish**
5. Build and flash to your STM32 board

---

## Related Resources

- 📖 [Full Tutorial — Controllerstech](https://controllerstech.com/low-power-modes-in-stm32/)
- 📁 [Stop Mode Project](../stop-mode/)
- 📁 [Standby Mode Project](../standby-mode/)

---

## License

MIT License. Free to use and modify for personal and commercial projects.
