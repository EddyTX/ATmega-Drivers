#pragma once

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// Configura?ii Sistem
#define SYSTEM_TICK_MS 1UL
#define UART_RX_BUFFER_SIZE 128
#define UART_TX_BUFFER_SIZE 128
#define TIMER_MAX_COUNT 3
#define HTTP_REQUEST_BUFFER_SIZE 128
#define MAX_HANDLERS 5

// --- SECURITATE ---
// Doar request-urile care con?in acest Host vor fi acceptate.
// Aceasta ac?ioneaz? ca o "Semn?tur? Digital?" a consolei autorizate.
#define ALLOWED_HOST "Siemens-Console"