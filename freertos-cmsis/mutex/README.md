# STM32 FreeRTOS Mutex — Priority Inheritance and Recursive Mutex

This project demonstrates how to use a mutex in FreeRTOS on STM32 using CMSIS-OS v2.
Four tasks compete for a shared resource using a mutex, and we observe priority inheritance
in real time. The project also demonstrates the recursive mutex and how it prevents deadlocks
in nested function calls.

---

## 📺 Tutorial

**[Watch the full video tutorial on YouTube →](https://youtu.be/ExquwgiF3sw)**

**[Read the full written guide →](https://controllerstech.com/stm32-freertos-mutex/)**

---

## 📋 Overview

Using a semaphore to protect shared resources between tasks of different priorities can lead
to priority inversion — a situation where a high priority task ends up waiting for a low
priority task because a medium priority task keeps preempting it. This project shows how a
mutex fixes this automatically using priority inheritance. It also demonstrates the recursive
mutex and how it prevents deadlocks when the same task needs to acquire the same mutex
more than once.

- **LPT** — Low priority task. Acquires the mutex and simulates resource usage
- **MPT** — Medium priority task. Runs without the mutex to demonstrate priority inversion
- **HPT** — High priority task. Waits for the mutex held by LPT
- **VHPT** — Very high priority task. Monitors and prints the priorities of all three tasks

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
| Mutex | `myMutex01Handle` — Dynamic allocation |
| Recursive Mutex | `myRecursiveMutex01Handle` — Dynamic allocation |
| UART | LPUART1 — PG7 (TX), PG8 (RX) |
| Baud Rate | 115200, 8N1 |

### Task Configuration

| Task | Priority | Stack Size | Role |
|------|----------|------------|------|
| LPT | Below Normal | 512 words | Acquires mutex, simulates resource usage |
| MPT | Normal | 512 words | Runs without mutex, used to show priority inversion |
| HPT | Above Normal | 512 words | Waits for the mutex held by LPT |
| VHPT | High | 512 words | Prints priorities of all tasks every 50ms |

---

## 💻 Task Code

### Mutex — LPT
```c
void StartLPT(void *argument)
{
    uint32_t wait;

    for(;;)
    {
        printf("Entered LPT, waiting for Mutex\n\n");
        osMutexAcquire(myMutex01Handle, osWaitForever);
        printf("LPT Acquired Mutex, using Resource\n\n");
        wait = 10000000;
        while (wait--);
        printf("LPT finished, released Mutex\n\n");
        osMutexRelease(myMutex01Handle);
        printf("LPT going to Sleep\n\n");
    }
}
```

### Mutex — MPT
```c
void StartMPT(void *argument)
{
    uint32_t wait;

    for(;;)
    {
        printf("Entered MPT, performing task\n\n");
        wait = 50000000;
        while (wait--);
        printf("MPT finished, going to Sleep\n\n");
        osDelay(500);
    }
}
```

### Mutex — HPT
```c
void StartHPT(void *argument)
{
    for(;;)
    {
        printf("Entered HPT, waiting for Mutex\n\n");
        osMutexAcquire(myMutex01Handle, osWaitForever);
        printf("HPT Acquired and now releasing Mutex\n\n");
        osMutexRelease(myMutex01Handle);
        printf("HPT going to Sleep\n\n");
        osDelay(200);
    }
}
```

### Mutex — VHPT
```c
void StartVHPT(void *argument)
{
    for(;;)
    {
        printf("Priorities ->-> LPT:%d MPT:%d HPT:%d\n\n",
                osThreadGetPriority(LPTHandle),
                osThreadGetPriority(MPTHandle),
                osThreadGetPriority(HPTHandle));
        osDelay(50);
    }
}
```

### Recursive Mutex — FunctionA and FunctionB
```c
void FunctionB(void)
{
    printf("FunctionB trying to lock mutex\n");
    osMutexAcquire(myRecursiveMutex01Handle, osWaitForever);
    printf("FunctionB acquired mutex\n");
    osDelay(100);
    osMutexRelease(myRecursiveMutex01Handle);
    printf("FunctionB released mutex\n");
}

void FunctionA(void)
{
    printf("FunctionA trying to lock mutex\n");
    osMutexAcquire(myRecursiveMutex01Handle, osWaitForever);
    printf("FunctionA acquired mutex\n");
    FunctionB();   // tries to lock the same mutex again
    osMutexRelease(myRecursiveMutex01Handle);
    printf("FunctionA released mutex\n\n");
}

void StartLPT(void *argument)
{
    for(;;)
    {
        FunctionA();
        osDelay(5000);
    }
}
```

---

## 🔗 Related Projects

* [FreeRTOS Stack Management and Overflow Detection](https://github.com/controllerstech/STM32-HAL/tree/master/freertos-cmsis/Stack-Management)
* [FreeRTOS Software Timers (Periodic & One-Shot)](https://github.com/controllerstech/STM32-FreeRTOS-Software-Timers)
* [FreeRTOS Event Flags](https://github.com/controllerstech/STM32-FreeRTOS-Event-Flags)
* [FreeRTOS Semaphores — Binary and Counting](https://github.com/controllerstech/STM32-HAL/tree/master/freertos-cmsis/semaphores-binary-counting)
* [FreeRTOS Queue — Inter-Task Communication](https://github.com/controllerstech/STM32-HAL/tree/master/freertos-cmsis/queue-inter-task-communication)
* [FreeRTOS Multiple Tasks and Priorities](https://github.com/controllerstech/STM32-HAL/tree/master/freertos-cmsis/multiple-tasks-priorities)
* [FreeRTOS Setup and Blink LED](https://github.com/controllerstech/STM32-HAL/tree/master/freertos-cmsis/setup-led-blink)

---

## 📜 License

MIT License. Free to use and modify for personal and commercial projects.
