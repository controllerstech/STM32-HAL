# UART Blocking & Interrupt Receive – STM32 HAL Example

This project demonstrates how to receive data over UART using **Blocking** and **Interrupt** modes on STM32 microcontrollers using the HAL driver.

Blocking mode is simple but halts the CPU while waiting for incoming bytes — causing it to miss data arriving outside the receive window. Interrupt mode solves this by receiving data in the background, freeing the CPU to handle other tasks simultaneously. This is the third tutorial in the STM32 UART series.

---

## Features Covered

- Limitations of blocking mode for UART reception
- Receive fixed-length data using `HAL_UART_Receive()` (Blocking mode)
- Receive fixed-length data using `HAL_UART_Receive_IT()` (Interrupt mode)
- Using `HAL_UART_RxCpltCallback()` to process received data
- Re-arming the interrupt after each receive cycle
- Receiving **variable-length / unknown-length** data using a terminating character (`\n`)
- Buffer management with a rolling index for byte-by-byte reception

---

## Reception Mode Comparison

| Feature | Blocking Mode | Interrupt Mode |
|---|---|---|
| CPU Usage | High – CPU waits for each byte | Low – CPU free between interrupts |
| Real-Time Performance | Poor – other tasks delayed | Excellent – tasks run concurrently |
| Data Length Requirement | Must be known in advance | Can handle fixed or variable length |
| Implementation Complexity | Simple | Moderate – requires callbacks |
| Risk of Missing Data | High – data arriving outside window is lost | Low – interrupt fires on each byte |
| HAL Function | `HAL_UART_Receive()` | `HAL_UART_Receive_IT()` |

---

## Tested On

This example has been tested on the following STM32 boards:

- STM32F103 (F1 series)
- STM32F407 (F4 series)
- STM32H743 (H7 series)

The core HAL code is portable across most STM32 MCUs with minor clock and peripheral adjustments.

---

## Development Setup

- **IDE:** STM32CubeIDE
- **HAL Drivers:** Included via CubeMX
- **Project Included:** Complete STM32CubeIDE project folder

---

## Connections

If using an **external USB-TTL converter:**

| STM32 Pin | USB-TTL Pin |
|-----------|-------------|
| TX | RX |
| RX | TX |
| GND | GND |

> ⚠️ TX connects to RX and RX connects to TX (cross connection).

![UART Connection](images/connection.webp)

If using an **onboard ST-Link** (e.g., Nucleo boards with Virtual COM Port), no external wiring is required.

---

## CubeMX Configuration

### Blocking Mode
- Enable UART in **Asynchronous** mode
- Set baud rate to **115200**, 8N1
- No additional configuration needed

### Interrupt Mode
- Same UART configuration as Blocking mode
- Additionally, **enable the UART global interrupt** in the NVIC tab

---

## How It Works

### Blocking Mode

```c
while (1)
{
    HAL_UART_Receive(&huart2, RxData, 5, 1000);

    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(1000);
}
```

`HAL_UART_Receive()` waits for exactly 5 bytes or 1 second timeout, whichever comes first. If data arrives while the CPU is inside `HAL_Delay()`, it is lost entirely. This makes blocking mode unreliable for anything beyond the simplest use cases.

---

### Interrupt Mode – Fixed Length

```c
// Called once before the while loop to arm the interrupt
HAL_UART_Receive_IT(&huart2, RxData, 5);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // Process RxData here

    // Re-arm the interrupt for the next 5 bytes
    HAL_UART_Receive_IT(&huart2, RxData, 5);
}
```

`HAL_UART_Receive_IT()` returns immediately. The CPU continues executing other code and the callback fires once all 5 bytes have arrived. The interrupt is automatically disabled after each trigger, so it must be re-armed inside the callback.

---

### Interrupt Mode – Variable / Unknown Length

For data of unknown size, receive 1 byte at a time and watch for a terminating character (`\n`):

```c
uint8_t FinalData[20];
uint8_t RxData[20];
uint8_t temp[2];
int indx = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    memcpy(RxData + indx, temp, 1);
    if (++indx >= 20) indx = 0;
    HAL_UART_Receive_IT(&huart2, temp, 1);
}

int main()
{
    HAL_UART_Receive_IT(&huart2, temp, 1);

    while (1)
    {
        if (temp[0] == '\n')
        {
            memcpy(FinalData, RxData, indx);
            indx = 0;
        }

        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(1000);
    }
}
```

Each incoming byte triggers the callback, which appends it to `RxData` and immediately re-arms for the next byte. Once `\n` is detected in the main loop, the complete message is copied into `FinalData`.

> **Note:** This byte-by-byte approach works well for moderate data rates. At very high baud rates or large message sizes, the overhead of serving an interrupt per byte may cause data loss. For those cases, consider UART DMA with Idle Line detection (see Part 5 of the series).

---

## How to Use

1. Open the project in **STM32CubeIDE**
2. Choose the desired reception mode:
   - For **Blocking mode**: use `HAL_UART_Receive()` in the while loop
   - For **Interrupt mode (fixed length)**: call `HAL_UART_Receive_IT()` once before the loop and implement `HAL_UART_RxCpltCallback()`
   - For **Interrupt mode (variable length)**: set receive size to 1 and use a terminating character
3. Ensure matching CubeMX configuration (NVIC interrupt enabled for interrupt modes)
4. Build and flash the firmware
5. Open a serial terminal with matching settings (**115200 baud, 8N1**)
6. Send data and observe it being received in the `RxData` buffer via the debugger

---

## Result

- In **blocking mode**, the LED blinking rate is affected whenever data is expected — the CPU stalls for up to the full timeout duration even when no data arrives.
- In **interrupt mode**, the LED blinks consistently every 1 second regardless of incoming data, confirming the CPU is never blocked during reception.

---

## Full Tutorial and Explanation

Step-by-step explanation, diagrams, and code walkthrough available at:

👉 https://controllerstech.com/stm32-uart-3-receive-data-in-blocking-interrupt-mode/

---

## Related Tutorials

| Part | Topic | Link |
|------|-------|------|
| Part 1 | Configure UART & Transmit in Blocking Mode | https://controllerstech.com/stm32-uart-1-configure-uart-transmit-data/ |
| Part 2 | Transmit using Interrupt & DMA | https://controllerstech.com/stm32-uart-2-use-interrupt-dma-to-transmit-data/ |
| Part 4 | Receive Data using DMA | https://controllerstech.com/stm32-uart-4-receive-data-using-dma/ |
| Part 5 | Receive Data using IDLE Line Detection | https://controllerstech.com/stm32-uart-5-receive-data-using-idle-line/ |

---

## License

This example is provided for educational purposes under Controllerstech Guidelines.
