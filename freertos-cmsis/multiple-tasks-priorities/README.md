# FreeRTOS CMSIS-RTOS V2 — Multiple Tasks, Priorities & Preemption

This project demonstrates how to create and manage multiple FreeRTOS tasks with different priorities on the **STM32 Nucleo-L496ZG** using the **CMSIS-RTOS V2 API** in STM32CubeIDE.

It covers task preemption in action, task lifecycle control using suspend and resume, and an overview of the key CMSIS-RTOS V2 thread management functions.

This is **Part 2** of the STM32 FreeRTOS series on [Controllerstech](https://controllerstech.com).

---

## 📺 Tutorial

**[Watch the full video tutorial on YouTube →](https://youtu.be/tzWdUVcMUgU)**

**[Read the full written guide →](https://controllerstech.com/stm32-freertos-multiple-tasks-priorities-preemption/)**

---

## Hardware

- **Board:** STM32 Nucleo-L496ZG
- **LEDs:**
  - PB14 → Red LED (Task1)
  - PB7 → Blue LED (Task2)
  - PC7 → Green LED (Task3)

---

## Task Configuration

| Task  | Entry Function | Priority     | LED   | Delay  |
|-------|---------------|--------------|-------|--------|
| Task1 | StartTask1    | Low          | Red   | 1000ms |
| Task2 | StartTask2    | Below Normal | Blue  | 500ms  |
| Task3 | StartTask3    | Normal       | Green | 200ms  |
| Task4 | StartTask4    | Above Normal | —     | Controls Task3 lifecycle |

---

## What This Project Covers

### 1. Multiple Tasks with Different Priorities
Four FreeRTOS tasks run concurrently. Each LED task blinks at a different rate using `osDelay()`, which puts the task into the blocked state and allows lower priority tasks to run during that window.

### 2. Task Preemption
A blocking `while` loop is added to different tasks to observe how the scheduler responds:

- **While loop in Task1 (Low priority):** Task2 and Task3 preempt it freely. Their LEDs blink at normal rates.
- **While loop in Task3 (Normal priority):** Task1 and Task2 are forced to wait. All three LEDs appear to sync to Task3's cycle.

### 3. Task Lifecycle Control with Task4
Task4 uses `osThreadSuspend()` and `osThreadResume()` to control Task3:

```c
void StartTask4(void *argument)
{
  for(;;)
  {
    osDelay(1000);
    osThreadSuspend(Task3Handle);
    osDelay(5000);
    osThreadResume(Task3Handle);
  }
}
```

While Task3 is suspended, the Green LED stays frozen in its current state and Task1 and Task2 run freely without any interference.

---

## Project Setup

1. Clone or download this repository
2. Open STM32CubeIDE
3. Go to **File → Import → Existing Projects into Workspace**
4. Select the project folder and click **Finish**
5. Build the project and flash it to the Nucleo-L496ZG board

> **Note:** This project was created with STM32CubeIDE and uses the CMSIS-RTOS V2 wrapper over FreeRTOS. No changes to FreeRTOS configuration are needed.

---

## 🔗 Related Projects

* [FreeRTOS Stack Management and Overflow Detection](https://github.com/controllerstech/STM32-HAL/tree/master/freertos-cmsis/Stack-Management)
* [FreeRTOS Software Timers (Periodic & One-Shot)](https://github.com/controllerstech/STM32-FreeRTOS-Software-Timers)
* [FreeRTOS Event Flags](https://github.com/controllerstech/STM32-FreeRTOS-Event-Flags)
* [FreeRTOS Mutex — Priority Inheritance and Recursive Mutex](https://github.com/controllerstech/STM32-FreeRTOS-Mutex)
* [FreeRTOS Semaphores — Binary and Counting](https://github.com/controllerstech/STM32-HAL/tree/master/freertos-cmsis/semaphores-binary-counting)
* [FreeRTOS Queue — Inter-Task Communication](https://github.com/controllerstech/STM32-HAL/tree/master/freertos-cmsis/queue-inter-task-communication)
* [FreeRTOS Setup and Blink LED](https://github.com/controllerstech/STM32-HAL/tree/master/freertos-cmsis/setup-led-blink)

---

## License

MIT License. Free to use and modify for personal and commercial projects.
