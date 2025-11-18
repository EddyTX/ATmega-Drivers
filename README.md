# ATmega HTTP Server

A simple, modular HTTP server implementation for ATmega328P (Arduino Uno/Nano), running over a custom non-blocking UART driver.

## ⚙️ Quick Setup
* **Microcontroller:** ATmega328P
* **Frequency:** 16MHz
* **Connection:** Serial (UART) at **9600 Baud**

---

## 📡 API Command List

All requests must be sent via UART.

```text
=== HTTP COMMAND LIST (ATmega Server) ===
Note: All commands must include "HTTP/1.1" at the end.

-------------------------------------------------------
1. ADC (Analog Sensor Reading)
-------------------------------------------------------
Format: GET /adc/read/<channel> HTTP/1.1
Channels: 0 - 7

Exemple:
GET /adc/read/0 HTTP/1.1    (Read pin A0)
GET /adc/read/1 HTTP/1.1    (Read pin A1)
GET /adc/read/5 HTTP/1.1    (Read pin A5)

-------------------------------------------------------
2. GPIO (Digital Pin Control)
-------------------------------------------------------
Format: GET /gpio/toggle/<port>/<pin> HTTP/1.1
Format: GET /gpio/set/<port>/<pin>/<level> HTTP/1.1

Ports: B, C, D
Levels: high, low

Exemple (for built-in LED - Port B, Pin 5):
GET /gpio/toggle/B/5 HTTP/1.1      (Toggle state On/Off)
GET /gpio/set/B/5/high HTTP/1.1    (Turn LED ON)
GET /gpio/set/B/5/low HTTP/1.1     (Turn LED OFF)

-------------------------------------------------------
3. SYSTEM (Timer & Test)
-------------------------------------------------------
GET /timer/elapsed HTTP/1.1        (Get system uptime in seconds)
GET /msg HTTP/1.1                  (Connectivity check - Hello World)