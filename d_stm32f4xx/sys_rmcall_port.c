#include "sys_rmcall_port.h"

#if defined(CMODULE_USE_RMCALL) && (CMODULE_USE_RMCALL > 0)

#include <sys_rmcall.h>

#define SYSLOG_TAG "RMCALL"
#define SYSLOG_LVL RMCALL_SYSLOG_LVL
#include <inc_syslog.h>

extern rmcall_t rmcall_host;

mstatus_t RMCALL_HOST_Tx(void *_data, uint32_t _dataSize)
{
    return HAL_UART_Transmit_IT(&huart3, (uint8_t*)_data, _dataSize);
}
mstatus_t RMCALL_HOST_Rx(void *_data, uint32_t _dataSize)
{
    return HAL_UART_Receive_IT(&huart3, (uint8_t*)_data, _dataSize);
}
void RMCALL_HOST_TxAbort(void)
{
    HAL_UART_AbortTransmit_IT(&huart3);
}
void RMCALL_HOST_RxAbort(void)
{
    HAL_UART_AbortReceive_IT(&huart3);
}

void RMCALL_HOST_TxCpltCallback(UART_HandleTypeDef *huart)
{
    RMCALL_TxIsr(&rmcall_host);
}

void RMCALL_HOST_RxCpltCallback(UART_HandleTypeDef *huart)
{
    RMCALL_RxIsr(&rmcall_host);
}

#endif // CMODULE_USE_RMCALL