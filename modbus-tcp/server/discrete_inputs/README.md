# STM32 Modbus TCP Server – Read Discrete Inputs

This project demonstrates how to configure the **STM32 Nucleo H755ZI** as a **Modbus TCP server** using the Mongoose networking library. The server responds to FC02 read requests from a Modbus TCP client and returns the real-time state of 10 physical discrete inputs connected to the GPIO pins.

This is **Part 1** of the STM32 Modbus TCP series.

---

## Full Tutorial and Explanation

Step-by-step explanation, CubeMX screenshots, and video walkthrough available at:

- https://controllerstech.com/stm32-modbus-tcp-server-read-discrete-inputs/
- https://youtu.be/4-yqfzoIIlQ

---

## Features Covered

- Generating the base project using the Mongoose web UI wizard
- Configuring 10 GPIO input pins in STM32CubeMX with pull-up mode
- Writing a custom Modbus handler for FC02 (Read Discrete Inputs)
- Registering the handler using `mongoose_set_modbus_handler()`
- Testing with Simply Modbus TCP Client on Windows
- Analyzing raw Modbus TCP request and response frames byte by byte
- Setting up the Mongoose web UI with WebSocket for real-time input monitoring

---

## How Discrete Inputs Work

Each GPIO pin is connected to a switch and GND. Pull-up resistors are enabled in CubeMX.

| Switch State | Pin State | Modbus Value |
|---|---|---|
| Open | Pulled high to VDD | 1 |
| Closed (connected to GND) | Low | 0 |

---

## Tested On

| Board | Core Used | Interface |
|---|---|---|
| STM32 Nucleo H755ZI | Cortex-M7 (CM7) | Ethernet (RMII) |

---

## IP Address Configuration

| Device | IP Address | Port |
|---|---|---|
| STM32 (Server) | `192.168.0.10` | `502` |
| PC (Client) | `192.168.0.2` | — |

Static IP is configured in `mongoose_config.h`:

```c
#define MG_TCPIP_IP   MG_IPV4(192, 168, 0, 10)
#define MG_TCPIP_GW   MG_IPV4(192, 168, 0, 1)
#define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)
```

---

## How It Works

```
1. Mongoose wizard generates the base project with Modbus TCP server enabled
2. STM32CubeMX configures 10 GPIO pins as inputs with pull-up mode
3. input_t array maps each input to its GPIO port and pin
4. READ_INPUTS(uint16_t i) reads the physical state of each pin using HAL_GPIO_ReadPin()
5. my_modbus_handler() handles incoming FC02 requests from the client
6. mongoose_set_modbus_handler() registers the custom handler with Mongoose
7. Modbus TCP client sends a read request → server responds with 10 input states
8. WebSocket reporter sends input states to the web UI every 200ms
```

---

## Key Code

### Input Structure and Array

```c
typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} input_t;

input_t inputs[10] = {
    {INPUT1_GPIO_Port, INPUT1_Pin},
    {INPUT2_GPIO_Port, INPUT2_Pin},
    {INPUT3_GPIO_Port, INPUT3_Pin},
    {INPUT4_GPIO_Port, INPUT4_Pin},
    {INPUT5_GPIO_Port, INPUT5_Pin},
    {INPUT6_GPIO_Port, INPUT6_Pin},
    {INPUT7_GPIO_Port, INPUT7_Pin},
    {INPUT8_GPIO_Port, INPUT8_Pin},
    {INPUT9_GPIO_Port, INPUT9_Pin},
    {INPUT10_GPIO_Port, INPUT10_Pin},
};
```

### Read Input Function

```c
int READ_INPUTS(uint16_t i) {
    return HAL_GPIO_ReadPin(inputs[i].port, inputs[i].pin);
}
```

### Modbus Handler (FC02)

```c
void my_modbus_handler(struct mg_modbus_req *req) {
  if (req->func == MG_MODBUS_FUNC_READ_DISCRETE_INPUTS) {
    for (uint16_t i = 0; i < req->len; i++) {
      req->u.bits[i] = READ_INPUTS(req->addr + i);
    }
  }
  else {
    req->error = MG_MODBUS_ERR_DEVICE_FAILURE;
  }
}
```

### Web UI Getter Function

```c
static struct inputs dis_inputs[] = {
  {false},{false},{false},{false},{false},{false},{false},{false},{false},{false},
};

bool my_get_inputs(struct inputs *data, size_t i) {
  size_t array_size = sizeof(dis_inputs) / sizeof(dis_inputs[0]);
  if (i >= array_size) return false;

  for (int j =0; j<array_size; j++)
  {
	  dis_inputs[j].level = READ_INPUTS(j);
  }
  *data = dis_inputs[i];  // Sync with your device
  return true;
}
```

### Registration in main()

```c
mongoose_set_modbus_handler(my_modbus_handler);
mongoose_set_http_handlers("inputs", my_get_inputs, NULL);
mongoose_add_ws_reporter(200, "inputs");
```

---

## Modbus TCP Frame – FC02

### Request (Client → Server)

| Field | Size | Value |
|---|---|---|
| Transaction ID | 2 bytes | 0x0001 |
| Protocol ID | 2 bytes | 0x0000 |
| Length | 2 bytes | 0x0006 |
| Unit ID | 1 byte | 0x0A |
| Function Code | 1 byte | 0x02 |
| Start Address | 2 bytes | 0x0000 |
| Quantity | 2 bytes | 0x000A |

### Response (Server → Client)

| Field | Size | Value |
|---|---|---|
| Transaction ID | 2 bytes | 0x0001 |
| Protocol ID | 2 bytes | 0x0000 |
| Length | 2 bytes | 0x0005 |
| Unit ID | 1 byte | 0x0A |
| Function Code | 1 byte | 0x02 |
| Byte Count | 1 byte | 0x02 |
| Input Data | 2 bytes | states |

---

## Modbus Client Configuration (Simply Modbus TCP)

| Setting | Value |
|---|---|
| Mode | TCP |
| Server IP | 192.168.0.10 |
| Port | 502 |
| Slave ID | 10 |
| Function Code | 2 (Read Discrete Inputs) |
| Starting Address | 10001 |
| Quantity | 10 |

---

## Common Errors and Fixes

| Symptom | Likely Cause | Fix |
|---|---|---|
| Client gets no response | Wrong IP or port | Verify static IP matches client configuration |
| All inputs read 0 | Pull-up not enabled | Set GPIO mode to Pull-up in CubeMX |
| Handler never called | Handler not registered | Call `mongoose_set_modbus_handler()` after `mongoose_init()` |
| Web UI not updating | WebSocket not enabled | Enable WebSocket in Mongoose wizard settings |
| Build errors after regeneration | Modified Mongoose folder | Never edit files inside the Mongoose folder directly |

---

## Related Tutorials

| Part | Topic | Link |
|---|---|---|
| Prerequisite | Modbus TCP Protocol Explained | https://controllerstech.com/modbus-tcp-protocol-explained/ |
| Part 2 | Modbus TCP Server – Read/Write Coils | Coming Soon |
| Part 3 | Modbus TCP Server – Holding Registers | Coming Soon |

---

## License

This example is provided for educational purposes under Controllerstech Guidelines.
