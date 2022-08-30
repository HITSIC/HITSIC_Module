#ifndef D_MK66F18_SYS_RMCALL_PORT_H
#define D_MK66F18_SYS_RMCALL_PORT_H
#include "cmodule_common.h"
#include "inc_stdlib.h"

#if defined(CMODULE_USE_RMCALL) && (CMODULE_USE_RMCALL > 0)

//#define CMODULE_RMCALL_UART LPUART2

#define CMODULE_RMCALL_USE_HEARTBEAT (1U)

#if defined(CMODULE_RMCALL_USE_HEARTBEAT) && (CMODULE_RMCALL_USE_HEARTBEAT > 0)
// #define CMODULE_RMCALL_PITISR_TIMELAG 100
// #define CMODULE_RMCALL_HEARTBEAT_TIMELAG 1000
// #define CMODULE_RMCALL_HEARTBEAT_FAIL_TH 2000
// #define CMODULE_RMCALL_HEARTBEAT_Callback() HardFault_IRQHandler()
// #endif // ! CMODULE_RMCALL_USE_HEARTBEAT



#endif // ! CMODULE_RMCALL_USE_HEARTBEAT

#define CMODULE_RMCALL_HEADER_MAGIC 0x554768A0U

void RMCALL_Callback(LPUART_Type *base, lpuart_handle_t *handle, mstatus_t status, void *userData)
{
    userData = userData;

    if (mstatus_LPUART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing    = false;
    }

    if (mstatus_LPUART_RxIdle == status)
    {
        rxBufferEmpty = false;
        rxOnGoing     = false;
    }
}

mstatus_t RMCALL1_Tx(void *_data, uint32_t dataSize);
mstatus_t RMCALL1_Rx(void *_data, uint32_t dataSize);

#endif // CMODULE_USE_RMCALL

#endif // ! D_MK66F18_SYS_RMCALL_PORT_H
