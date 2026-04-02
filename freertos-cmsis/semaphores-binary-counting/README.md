# STM32 FreeRTOS Semaphores — Binary and Counting Semaphore

This project demonstrates how to use binary and counting semaphores in FreeRTOS on STM32
using CMSIS-OS v2. Three tasks compete for a shared resource using a binary semaphore,
and four tasks compete for three shared resources using a counting semaphore.

---

## 📺 Tutorial

**[Watch the full video tutorial on YouTube →](https://youtu.be/kK9vyS5BtlY)**

**[Read the full written guide →](https://controllerstech.com/stm32-freertos-semaphores/)**

---

## 📋 Overview

Using a shared peripheral like UART from multiple tasks without any protection leads to
corrupted data. This project shows how to use semaphores to control access to shared
resources safely. It also demonstrates priority inversion — a common RTOS pitfall —
and explains why a mutex is the proper fix.

- **LPT** — Low priority task. Acquires the semaphore and simulates resource usage
- **MPT** — Medium priority task. Used to demonstrate priority inversion
- **HPT** — High priority task. Competes for the semaphore with LPT
- **VHPT** — Very high priority task. Added for the counting semaphore demo

---

## 🛠 Hardware

- STM32L496 Nucleo board (any STM32 Nucleo board works with minor pin changes)
- USB cable for Virtual COM port (ST-LINK)

---

## ⚙️ CubeMX Configuration

| Parameter | Value |
|-----------|-------|
| RTOS | FreeRTOS (CMSIS-OS v2) |
| Heap Size | 10 KB |
| Binary Semaphore | `myBinarySem01Handle` — Initial state: Available |
| Counting Semaphore | `myCountingSem01Handle` — Max count: 3, Initial count: 3 |
| UART | LPUART1 — PG7 (TX), PG8 (RX) |
| Baud Rate | 115200, 8N1 |

### Task Configuration

| Task | Priority | Stack Size | Role |
|------|----------|------------|------|
| LPT | Below Normal | 512 words | Acquires semaphore, simulates resource usage |
| MPT | Normal | 512 words | Demonstrates priority inversion |
| HPT | Above Normal | 512 words | Competes for semaphore with LPT |
| VHPT | High | 512 words | Added for counting semaphore demo |

---

## 💻 Task Code

### Helper Functions
```c
int resource[3] = {111, 222, 333};
char *resourceOwner[3] = {"Free", "Free", "Free"};

int getResource(char *taskName)
{
    for (int i = 0; i < 3; i++)
    {
        if (strcmp(resourceOwner[i], "Free") == 0)
        {
            resourceOwner[i] = taskName;
            return i;
        }
    }
    return -1;
}

void releaseResource(int id)
{
    resourceOwner[id] = "Free";
}

void printResourceTable(void)
{
    printf("\nResource Table:\n");
    for (int i = 0; i < 3; i++)
    {
        printf("Resource %d -> %s\n", resource[i], resourceOwner[i]);
    }
    printf("\n");
}
```

### Binary Semaphore — LPT
```c
void StartLPT(void *argument)
{
    uint32_t wait;

    for(;;)
    {
        printf("Entered LPT\n\n");
        osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
        printf("LPT Using Resource\n\n");
        wait = 50000000;
        while (wait--);
        printf("LPT Finished, released Semaphore\n\n");
        osSemaphoreRelease(myBinarySem01Handle);
        printf("LPT going to Sleep\n\n");
        osDelay(500);
    }
}
```

### Binary Semaphore — MPT
```c
void StartMPT(void *argument)
{
    for(;;)
    {
        printf("Entered MPT\n\n");
        osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
        printf("MPT completed Task, released Semaphore\n\n");
        osSemaphoreRelease(myBinarySem01Handle);
        printf("MPT going to Sleep\n\n");
        osDelay(200);
    }
}
```

### Binary Semaphore — HPT
```c
void StartHPT(void *argument)
{
    for(;;)
    {
        printf ("Inside HPT\n\n");
        osDelay(1000);
    }
}
```

### Counting Semaphore — LPT
```c
void StartLPT(void *argument)
{
    int resID;

    for(;;)
    {
        printf("LPT waiting for resource\n");
        osSemaphoreAcquire(myCountingSem01Handle, osWaitForever);
        resID = getResource("LPT");
        printf("LPT accessing resource %d\n", resource[resID]);
        printResourceTable();
        osDelay(2000);
        printf("LPT finished using resource\n");
        releaseResource(resID);
        osSemaphoreRelease(myCountingSem01Handle);
        osDelay(1000);
    }
}
```

### Counting Semaphore — MPT
```c
void StartMPT(void *argument)
{
    int resID;

    for(;;)
    {
        printf("MPT waiting for resource\n");
        osSemaphoreAcquire(myCountingSem01Handle, osWaitForever);
        resID = getResource("MPT");
        printf("MPT accessing resource %d\n", resource[resID]);
        printResourceTable();
        osDelay(2000);
        printf("MPT finished using resource\n");
        releaseResource(resID);
        osSemaphoreRelease(myCountingSem01Handle);
        osDelay(1000);
    }
}
```

### Counting Semaphore — HPT
```c
void StartHPT(void *argument)
{
    int resID;

    for(;;)
    {
        printf("HPT waiting for resource\n");
        osSemaphoreAcquire(myCountingSem01Handle, osWaitForever);
        resID = getResource("HPT");
        printf("HPT accessing resource %d\n", resource[resID]);
        printResourceTable();
        osDelay(2000);
        printf("HPT finished using resource\n");
        releaseResource(resID);
        osSemaphoreRelease(myCountingSem01Handle);
        osDelay(1000);
    }
}
```

### Counting Semaphore — VHPT
```c
void StartVHPT(void *argument)
{
    int resID;

    for(;;)
    {
        printf("VHPT waiting for resource\n");
        osSemaphoreAcquire(myCountingSem01Handle, osWaitForever);
        resID = getResource("VHPT");
        printf("VHPT accessing resource %d\n", resource[resID]);
        printResourceTable();
        osDelay(2000);
        printf("VHPT finished using resource\n");
        releaseResource(resID);
        osSemaphoreRelease(myCountingSem01Handle);
        osDelay(1000);
    }
}
```

---

## 🔗 Related Projects

* [FreeRTOS Stack Management and Overflow Detection](https://github.com/controllerstech/STM32-HAL/tree/master/freertos-cmsis/Stack-Management)
* [FreeRTOS Software Timers (Periodic & One-Shot)](https://github.com/controllerstech/STM32-FreeRTOS-Software-Timers)
* [FreeRTOS Event Flags](https://github.com/controllerstech/STM32-FreeRTOS-Event-Flags)
* [FreeRTOS Mutex — Priority Inheritance and Recursive Mutex](https://github.com/controllerstech/STM32-FreeRTOS-Mutex)
* [FreeRTOS Queue — Inter-Task Communication](https://github.com/controllerstech/STM32-HAL/tree/master/freertos-cmsis/queue-inter-task-communication)
* [FreeRTOS Multiple Tasks and Priorities](https://github.com/controllerstech/STM32-HAL/tree/master/freertos-cmsis/multiple-tasks-priorities)
* [FreeRTOS Setup and Blink LED](https://github.com/controllerstech/STM32-HAL/tree/master/freertos-cmsis/setup-led-blink)

---

## 📜 License

MIT License. Free to use and modify for personal and commercial projects.
```
