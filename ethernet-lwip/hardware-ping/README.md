# STM32 Ethernet – Hardware Setup, LWIP & Ping Test

This project demonstrates how to bring up Ethernet on an STM32 microcontroller from scratch — covering hardware connections, CubeMX configuration, LWIP stack setup with a static IP, MPU cache coherency configuration (for Cortex-M7 devices), and verifying the connection with a ping test.

This is **Part 1** of the STM32 Ethernet (LWIP) series.

---

## Features Covered

- Overview of MAC and PHY roles in STM32 Ethernet
- RMII vs MII hardware interface selection
- CubeMX Ethernet peripheral configuration (PHY address, pin verification)
- Enabling LWIP middleware with a static IP address
- Heap size and memory address configuration for LWIP
- MPU configuration to fix cache coherency issues on Cortex-M7 (F7/H7)
- Flash linker script modification for DMA descriptor placement
- PC-side network configuration for direct cable connection
- Ping test to verify end-to-end Ethernet connectivity

---

## Tested On

| Board | Series | Interface |
|-------|--------|-----------|
| Nucleo F207ZG | F2 | RMII |
| Discovery F7508 | F7 | RMII |
| Discovery H745 | H7 | MII |

The core approach is applicable to most STM32 boards with an onboard PHY or external LAN module. PHY address, pin mapping, and MPU settings will vary by board.

---

## Development Setup

- **IDE:** STM32CubeIDE
- **Middleware:** LWIP (enabled via CubeMX)
- **HAL Drivers:** Included via CubeMX code generation
- **Project Included:** Complete STM32CubeIDE project folder

---

## Hardware Connections

### Using an onboard LAN port (e.g., Nucleo, Discovery boards)
No additional wiring needed. The PHY chip is already connected on the board.

### Using an external LAN module

Connect following the RMII signals between STM32 and PHY:

| STM32 Pin | PHY Pin |
|-----------|---------|
| ETH_RMII_REF_CLK | REFCLK |
| ETH_RMII_CRS_DV | CRS_DV |
| ETH_RMII_RXD0 | RXD0 |
| ETH_RMII_RXD1 | RXD1 |
| ETH_RMII_TX_EN | TXEN |
| ETH_RMII_TXD0 | TXD0 |
| ETH_RMII_TXD1 | TXD1 |
| ETH_MDC | MDC |
| ETH_MDIO | MDIO |
| GND | GND |
| 3.3V | VCC |

> ⚠️ Always cross-check the CubeMX-generated pin assignments against your board's schematic. Incorrect pin mapping is one of the most common reasons Ethernet fails to initialise.

---

## CubeMX Configuration

### Step 1 – Ethernet Peripheral

- Enable **ETH** in the **Connectivity** tab
- Select **RMII** or **MII** depending on your board's hardware
- Set the **PHY Address**:
  - `0` for onboard LAN ports
  - `1` for external PHY modules
- Verify all generated pins match your board schematic

### Step 2 – Memory Addresses (H7 / F7 boards only)

Set DMA descriptor and buffer addresses in SRAM. Example for H745 Discovery:

| Region | Address |
|--------|---------|
| RX DMA Descriptor | `0x30000000` |
| TX DMA Descriptor | `0x30000080` |
| RX Buffer Pool | `0x30000100` |

> If your board does not expose memory address fields in CubeMX, skip this step.

### Step 3 – LWIP Middleware

- Enable **LWIP** under **Middleware**
- Disable **DHCP** and set a **static IP** (e.g., `192.168.0.123`)
- Set subnet mask to `255.255.255.0`
- In the **Key Options** tab, allocate heap size (e.g., `5120` bytes) and set heap address (e.g., `0x30004900`)
- In the **Platform Settings** tab, set PHY to **LAN8742** (or your board's PHY)

> Make sure I-Cache and D-Cache are enabled in System Core, otherwise LWIP cannot be enabled in CubeMX.

### Step 4 – MPU Configuration (Cortex-M7 only — F7 / H7)

Configure the MPU to mark the Ethernet DMA region as **non-cacheable** to prevent cache coherency issues between the CPU and DMA:

| Setting | Value |
|---------|-------|
| Region Base Address | `0x30000000` |
| Region Size | 32 KB |
| Access Permission | Full Access |
| Cacheable | Disabled |
| Bufferable | Disabled |
| Shareable | Enabled |

> Skipping MPU configuration on Cortex-M7 devices will cause hard faults or dropped packets at runtime.

---

## Linker Script Modification

After generating the project, add the following section to your `.ld` flash script to place the LWIP DMA descriptors at the correct SRAM addresses.

**For H745 Discovery (RAM_D2):**

```ld
.lwip_sec (NOLOAD) : {
  . = ABSOLUTE(0x30000000);
  *(.RxDescripSection)

  . = ABSOLUTE(0x30000080);
  *(.TxDescripSection)

  . = ABSOLUTE(0x30000100);
  *(.Rx_PoolSection)
} >RAM_D2
```

**For F7508 Discovery (RAM):**

```ld
.lwip_sec (NOLOAD) : {
  . = ABSOLUTE(0x2004C000);
  *(.RxDecripSection)

  . = ABSOLUTE(0x2004C0A0);
  *(.TxDecripSection)
} >RAM
```

These section names (`RxDecripSection`, `TxDecripSection`, `Rx_PoolSection`) are referenced in the CubeMX-generated `LWIP/Target/ethernetif.c` file and must match exactly.

---

## HAL Code

The `main.c` is minimal — LWIP handles all Ethernet processing internally via `MX_LWIP_Process()`.

```c
int main(void)
{
    int wait = 10000000;
    while (wait-- > 0);  // Allow PHY to stabilise before init

    MPU_Config();
    SCB_EnableICache();
    SCB_EnableDCache();
    HAL_Init();

    HAL_Delay(1000);

    SystemClock_Config();
    MX_GPIO_Init();
    MX_LWIP_Init();

    while (1)
    {
        MX_LWIP_Process();  // Drives the entire LWIP stack
    }
}
```

> `MX_LWIP_Process()` handles all internal LWIP timers, ARP, ICMP (ping), and protocol processing. It must be called continuously and must never be blocked or delayed inside the while loop.

---

## PC Configuration (Direct Cable Connection)

If connecting the STM32 directly to a PC without a router, manually assign a static IP on the PC's Ethernet adapter within the same subnet as the STM32.

**Windows:** Control Panel → Network Adapters → Ethernet → IPv4 Properties
- IP Address: `192.168.0.100`
- Subnet Mask: `255.255.255.0`
- Default Gateway: leave blank

**Mac:** System Settings → Network → Ethernet → Manual
- IP Address: `192.168.0.100`
- Subnet Mask: `255.255.255.0`

> If connecting via a router, no PC-side configuration is needed — just ensure both devices are on the same subnet.

---

## How to Use

1. Open the project in **STM32CubeIDE**
2. Verify pin assignments in CubeMX match your board's schematic
3. Update the linker script with the correct DMA descriptor addresses for your board
4. Build and flash the firmware
5. Connect the Ethernet cable between STM32 and PC (or router)
6. Configure the PC's static IP if connecting directly (see above)
7. Open a terminal or CMD and ping the STM32:

```
ping 192.168.0.123
```

A successful ping confirms Ethernet is fully operational.

---

## Result

A working setup produces continuous ping replies:

```
Reply from 192.168.0.123: bytes=32 time<1ms TTL=255
Reply from 192.168.0.123: bytes=32 time<1ms TTL=255
Reply from 192.168.0.123: bytes=32 time<1ms TTL=255
```

The image below shows the ping test result confirming successful Ethernet communication:

![Ping Test Result](images/ping_result.avif)

---

## Common Errors & Fixes

| Symptom | Likely Cause | Fix |
|---------|-------------|-----|
| No PHY link / RJ45 LED off | Wrong PHY address or pin mapping | Check schematic, correct PHY address in CubeMX |
| Ping request timed out | IP mismatch or wrong subnet | Ensure PC and STM32 are on the same subnet |
| Hard fault on startup | MPU not configured (Cortex-M7) | Configure SRAM region as non-cacheable in MPU |
| Unstable / dropped packets | Cache coherency issue | Verify MPU covers the full DMA descriptor region |
| LWIP won't enable in CubeMX | I-Cache / D-Cache not enabled | Enable both caches in System Core settings |
| Works in debug, fails standalone | Initialisation timing issue | Add startup delay before `MPU_Config()` and `HAL_Init()` |

---

## Full Tutorial and Explanation

Step-by-step explanation, schematics, CubeMX screenshots, and video walkthrough available at:

👉 https://controllerstech.com/stm32-ethernet-hardware-cubemx-lwip-ping/

---

## Related Tutorials

| Part | Topic | Link |
|------|-------|------|
| Part 2 | UDP Server | https://controllerstech.com/stm32-ethenret-2-udp-server/ |
| Part 3 | UDP Client | https://controllerstech.com/stm32-ethernet-3-udp-client/ |
| Part 4 | TCP Server | https://controllerstech.com/stm32-ethernet-4-tcp-server/ |
| Part 5 | TCP Client | https://controllerstech.com/stm32-ethernet-5-tcp-client/ |
| Part 6 | HTTP Web Server (Simple) | https://controllerstech.com/stm32-ethernet-6-http-webserver-simple/ |
| Part 6.1 | HTTP Web Server (SSI) | https://controllerstech.com/stm32-ethernet-6-1-http-webserver-ssi/ |
| Part 6.2 | HTTP Web Server (CGI) | https://controllerstech.com/stm32-ethernet-6-2-http-webserver-cgi/ |

---

## License

This example is provided for educational purposes under Controllerstech Guidelines.
