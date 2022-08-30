#ifndef D_IMXRT1052_SYS_RMCALL_PORT_H
#define D_IMXRT1052_SYS_RMCALL_PORT_H
#include "hitsic_common.h"
#include "peripherals.h"
//#include "inc_stdlib.h"

#if defined(CMODULE_USE_RMCALL) && (CMODULE_USE_RMCALL > 0)

#define RMCALL_UART_INST BOARD_UART_RMCALL_PERIPHERAL
extern lpuart_handle_t BOARD_UART_RMCALL_handle;

#define RMCALL_HEADER_MAGIC (0x554768A0U)
#define RMCALL_SYSLOG_LVL (3U)
#define RMCALL_PUBLIC_BUF_SIZE (256U)
#define kStatusGroup_RMCALL (201U)

#define RMCALL_TRAILER_CRC32 (1U)

// TODO: implement this
#define RMCALL_USE_GENERIC_HANDLE (0U) // a generic handle, called when no maching id or parity error.

status_t RMCALL_HOST_Tx(void *_data, uint32_t _dataSize);
status_t RMCALL_HOST_Rx(void *_data, uint32_t _dataSize);
void RMCALL_HOST_TxAbort(void);
void RMCALL_HOST_RxAbort(void);

void RMCALL_HOST_CpltCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData);

#endif // CMODULE_USE_RMCALL

#endif // ! D_IMXRT1052_SYS_RMCALL_PORT_H
