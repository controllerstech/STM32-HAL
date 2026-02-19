# UART Blocking Transmit – STM32 HAL Example

This project demonstrates how to configure and use UART in **blocking (polling) mode** for data transmission on STM32 microcontrollers using the HAL driver.

In blocking mode, the CPU actively waits until data transmission is complete before proceeding. This is simple to implement and useful for basic UART usage.

---

## Features Covered

- Basic UART initialization using STM32CubeMX / HAL
- Transmit data using `HAL_UART_Transmit()`
- No interrupts or DMA required
- Suitable for simple communication and learning

---

## Tested On

This example has been tested on the following STM32 boards:

- **STM32F103** (F1 series)
- **STM32F407** (F4 series)
- **STM32H743** (H7 series)

The core HAL code is portable across most STM32 MCUs with minor clock/peripheral adjustments.

---

## Development Setup

- **IDE:** STM32CubeIDE
- **HAL Drivers:** Included via CubeMX
- **Project Included:** Complete STM32CubeIDE project folder

---

## How to Use

1. Open the project in **STM32CubeIDE**
2. Configure the UART peripheral as needed
3. Build and flash to your STM32 board
4. Observe transmit behavior via a serial terminal

---

## Full Tutorial and Explanation

Step-by-step explanation, diagrams, and code walkthrough are available at:

👉 https://controllerstech.com/stm32-uart-1-configure-uart-transmit-data/

---

## License

This example is provided for educational purposes under Controllerstech Guidelines.
