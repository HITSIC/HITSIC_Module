#ifndef D_STM32F4XX_SYS_RMCALL_PORT_H
#define D_STM32F4XX_SYS_RMCALL_PORT_H
#include "cmodule_common.h"
#include "inc_stdlib.h"

#if defined(CMODULE_USE_RMCALL) && (CMODULE_USE_RMCALL > 0)

#include "usart.h"
#include "stm32f4xx_hal_uart.h"

#define RMCALL_SYSLOG_LVL (2U)

//#define CMODULE_RMCALL_UART LPUART2

#define CMODULE_RMCALL_PUBLIC_BUF_SIZE (256U)

#define CMODULE_RMCALL_HEADER_MAGIC (0x554768A0U)

#define mStatusGroup_RMCALL (201U)

mstatus_t RMCALL_HOST_Tx(void *_data, uint32_t _dataSize);
mstatus_t RMCALL_HOST_Rx(void *_data, uint32_t _dataSize);
void RMCALL_HOST_TxAbort(void);
void RMCALL_HOST_RxAbort(void);

void RMCALL_HOST_TxCpltCallback(UART_HandleTypeDef *huart);
void RMCALL_HOST_RxCpltCallback(UART_HandleTypeDef *huart);

#endif // CMODULE_USE_RMCALL

#endif // ! D_STM32F4XX_SYS_RMCALL_PORT_H
