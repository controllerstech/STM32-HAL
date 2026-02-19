# STM32 & ESP HAL/IoT Example Projects – Controllerstech

Welcome to the **STM32-HAL repository**!  
This repository contains hands-on **STM32 and ESP32/ESP8266 example projects**, covering HAL drivers, peripherals, FreeRTOS, Ethernet, IoT, Modbus TCP, and more.  
All projects are designed for beginners and professionals to learn embedded development efficiently.

Most STM32 examples are tested on **F1, F4, H7 series** boards. ESP32/ESP8266 examples use **ESP-IDF** or Arduino framework where applicable.

---

## Tutorials and Example Projects

### 1️⃣ UART Series
- Blocking Transmit  
- Interrupt Transmit/Receive  
- DMA Transmit/Receive  
- Idle Line Detection  
- Single Wire Communication  
- 1-Wire Protocol  
- LIN Protocol (Parts 1–3)  

### 2️⃣ ADC Series
- Single Channel Polling  
- Single Channel Interrupt/DMA  
- Multi-Channel DMA (Normal & Circular)  
- Multi-Channel without DMA  
- ADC Conversion Time  
- External Trigger & Injected Conversion  

### 3️⃣ Timer Series
- PWM Generation  
- Measure PWM Input  
- Encoder Mode  
- Timer Sync / Slave / Reset  
- 48-bit Counter  

### 4️⃣ FreeRTOS Series
- Task Creation & Priorities  
- Semaphores (Binary/Counting)  
- Queues  
- Mutex  
- Software Timers  

### 5️⃣ Ethernet & Networking
- W5500 TCP Server, DHCP, Static IP  
- STM32 LWIP Projects (HTTP Server, TCP/UDP)  
- Modbus TCP Examples  

### 6️⃣ IoT & ESP Series
- ESP8266 WiFi Projects with STM32 
- ESP8266 MQTT & IoT Projects  

### 7️⃣ STM32 Displays
- STM32 LVGL Series 
- TouchGFX Series  
- SPI and I2C Displays

### 8️⃣ STM32 Memories and Storage
- SPI and QSPI FLash Tutorials
- STM32 FLash Programming Series  
- SD Card Interfacing

---

## 🛠 Development Environment
- **STM32:** STM32CubeIDE, HAL Drivers (CubeMX generated)  
- **ESP:** ESP-IDF or Arduino framework  
- **Boards Tested:** STM32 F1, F4, H7; ESP32 DevKit, ESP8266 modules  
- **Project Structure:** Each folder contains a complete project with README and optional diagrams  

---

## 📁 Repository Structure
```
STM32-HAL/
├── uart/                  # UART communication examples (blocking, interrupt, DMA, LIN, etc.)
├── adc/                   # ADC projects (single channel, multi-channel, DMA, external triggers)
├── timers/                # Timer examples (PWM, input capture, encoder mode, timer sync)
├── freertos/              # FreeRTOS projects (tasks, semaphores, queues, mutex, software timers)
├── ethernet/              # Ethernet & networking projects
│   ├── w5500/             # W5500 Ethernet TCP/UDP, DHCP, static IP examples
│   └── lwip/              # STM32 LWIP stack projects (HTTP server, TCP/UDP communication)
├── iot/                   # IoT & ESP projects
│   ├── esp8266/           # ESP8266 WiFi projects and IoT examples
│   └── esp32/             # ESP32 projects (MQTT, TCP/UDP, IoT)
├── modbus/                # Modbus TCP / RTU projects
└── README.md              # Main repository overview
```
Each folder contains:
- Complete project files (CubeIDE / ESP-IDF / Arduino)  
- README for that example  
- Images, diagrams, or wiring references  

---

## 📘 Full Tutorials

Step-by-step guides and detailed explanations are available at **Controllerstech**:

[STM32 Tutorials](https://controllerstech.com/stm32-hal/)

---

## 📄 License

All examples are provided for **educational purposes**.  
You are free to use and adapt them for learning or personal projects.
