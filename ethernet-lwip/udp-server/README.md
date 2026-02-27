# STM32 Ethernet – UDP Server (Raw API)

This project demonstrates how to implement a **UDP Server** on STM32 using the LWIP Raw API. The server binds to a local IP and port, waits for data from any UDP client, and sends back a response — all without an RTOS.

This is **Part 2** of the STM32 Ethernet (LWIP) series.

---

## Features Covered

- Creating a UDP PCB (Protocol Control Block) with `udp_new()`
- Binding the server to a local IP and port with `udp_bind()`
- Registering a receive callback with `udp_recv()`
- Extracting the remote client IP and port inside the callback
- Allocating a transmit pbuf with `pbuf_alloc()`
- Copying response data into the pbuf with `pbuf_take()`
- Connecting to the client with `udp_connect()` and sending with `udp_send()`
- Disconnecting after each reply to accept new clients
- Proper pbuf memory management with `pbuf_free()`

---

## Tested On

| Board | Series | Interface |
|-------|--------|-----------|
| Nucleo F207ZG | F2 | RMII |
| Discovery F7508 | F7 | RMII |
| Discovery H745 | H7 | MII |

The CubeMX and LWIP configuration is identical to Part 1 of this series. Refer to the `hardware-ping` README for full hardware and CubeMX setup details.

---

## Development Setup

- **IDE:** STM32CubeIDE
- **Middleware:** LWIP Raw API (no RTOS required)
- **Test Tool:** Hercules (UDP Client) or any UDP terminal
- **Project Included:** Complete STM32CubeIDE project folder

---

## How UDP Server Works (LWIP Raw API)

UDP is connectionless — there is no handshake. The server simply binds to a port, waits for data to arrive, and replies directly to whoever sent it.

```
1. udp_new()     → Create a new UDP control block
2. udp_bind()    → Bind to local IP and port
3. udp_recv()    → Register callback for incoming data
4. [callback]    → Fires when data arrives from any client
5. pbuf_alloc()  → Allocate transmit buffer
6. pbuf_take()   → Copy response into buffer
7. udp_connect() → Point the PCB at the client's IP and port
8. udp_send()    → Send the response
9. udp_disconnect() → Free the connection for the next client
10. pbuf_free()  → Free both Tx and Rx pbufs
```

---

## Key Code

### Server Initialisation

```c
void udpServer_init(void)
{
    struct udp_pcb *upcb;
    err_t err;

    upcb = udp_new();

    ip_addr_t myIPADDR;
    IP_ADDR4(&myIPADDR, 192, 168, 0, 123);
    err = udp_bind(upcb, &myIPADDR, 7);  // Port 7

    if (err == ERR_OK)
    {
        udp_recv(upcb, udp_receive_callback, NULL);
    }
    else
    {
        udp_remove(upcb);
    }
}
```

- Binds to IP `192.168.0.123` on port `7`
- `udp_recv()` registers the callback — from this point the server is ready to receive
- If bind fails, the PCB is removed to free memory

---

### Receive Callback

```c
void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p,
                           const ip_addr_t *addr, u16_t port)
{
    struct pbuf *txBuf;

    /* Convert client IP to readable string */
    char *remoteIP = ipaddr_ntoa(addr);

    /* Build the response string */
    char buf[100];
    int len = sprintf(buf, "Hello %s From UDP SERVER\n", (char *)p->payload);

    /* Allocate transmit pbuf */
    txBuf = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);

    /* Copy response into pbuf */
    pbuf_take(txBuf, buf, len);

    /* Connect to the client and send */
    udp_connect(upcb, addr, port);
    udp_send(upcb, txBuf);

    /* Disconnect to allow new clients */
    udp_disconnect(upcb);

    /* Free both buffers */
    pbuf_free(txBuf);
    pbuf_free(p);
}
```

- `ipaddr_ntoa(addr)` converts the client IP from binary to a readable string (e.g. `"192.168.0.100"`)
- The incoming payload is embedded in the response string using `sprintf`
- A **new pbuf** is always allocated for the Tx buffer — never reuse the received `p` for sending
- `udp_disconnect()` after each reply is important — it resets the PCB's remote address so a new client can connect next time
- Both `txBuf` and `p` must be freed — forgetting either causes a memory leak

---

### main.c

```c
int main(void)
{
    MPU_Config();
    SCB_EnableICache();
    SCB_EnableDCache();
    HAL_Init();

    SystemClock_Config();
    MX_GPIO_Init();
    MX_LWIP_Init();

    udpServer_init();   // Start the UDP server

    while (1)
    {
        MX_LWIP_Process();
    }
}
```

---

## Result

The image below shows the UDP server receiving data from the Hercules UDP client and echoing back a response:

![UDP Server Result](images/udp_server_result.webp)

The client sends a message and the server replies with `Hello <client_message> From UDP SERVER`.

---

## Common Errors & Fixes

| Symptom | Likely Cause | Fix |
|---------|-------------|-----|
| No response from server | `udp_disconnect()` missing after send | Call `udp_disconnect()` so the PCB can accept new clients |
| Memory exhaustion after repeated messages | `pbuf_free()` not called | Always free both `txBuf` and `p` at the end of the callback |
| Intermittent ping loss (~50%) under traffic | Shared network with other devices | Expected on a busy switch; works cleanly on a direct cable connection |
| `SCB_EnableDCache` undefined symbol | Building on non-M7 device | Remove those calls — they are Cortex-M7 only |

---

## Full Tutorial and Explanation

Step-by-step explanation, CubeMX screenshots, and video walkthrough available at:

👉 https://controllerstech.com/stm32-ethenret-2-udp-server/

---

## Related Tutorials

| Part | Topic | Link |
|------|-------|------|
| Part 1 | Hardware Setup, LWIP & Ping Test | https://controllerstech.com/stm32-ethernet-hardware-cubemx-lwip-ping/ |
| Part 3 | UDP Client | https://controllerstech.com/stm32-ethernet-3-udp-client/ |
| Part 4 | TCP Server | https://controllerstech.com/stm32-ethernet-4-tcp-server/ |
| Part 5 | TCP Client | https://controllerstech.com/stm32-ethernet-5-tcp-client/ |
| Part 6 | HTTP Web Server (Simple) | https://controllerstech.com/stm32-ethernet-6-http-webserver-simple/ |
| Part 7 | UDP Server using NETCONN (RTOS) | https://controllerstech.com/udp-server-using-netconn-with-rtos-in-stm32/ |

---

## License

This example is provided for educational purposes under Controllerstech Guidelines.
