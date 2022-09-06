#include "sys_rmcall_port.h"

#if defined(CMODULE_USE_RMCALL) && (CMODULE_USE_RMCALL > 0)

#include <sys_rmcall.h>

#define SYSLOG_TAG "RMCALL"
#define SYSLOG_LVL RMCALL_SYSLOG_LVL
#include <inc_syslog.h>

extern rmcall_t rmcall_host;

uart_handle_t UART_BLE_handle;
uart_transfer_t UART_BLE_rxTransfer = {
  .rxData = NULL,
  .dataSize = 0
};
uart_transfer_t UART_BLE_txTransfer = {
  .data = NULL,
  .dataSize = 0,
};

rmcall_teleport_t teleport_host =
{
	RMCALL_HOST_Tx,
	RMCALL_HOST_Rx,
	RMCALL_HOST_TxAbort,
	RMCALL_HOST_RxAbort,
};
const rmcall_config_t config_host = 	{
		.teleport = &teleport_host
};

void RMCALL_PORT_Init(void)
{
	UART_TransferCreateHandle(CMODULE_RMCALL_UART, &UART_BLE_handle, RMCALL_HOST_CpltCallback, NULL);
	RMCALL_Init(&rmcall_host, &config_host);
}

mstatus_t RMCALL_HOST_Tx(void *_data, uint32_t _dataSize)
{
	UART_BLE_txTransfer.data     = (uint8_t *)_data;
	UART_BLE_txTransfer.dataSize = _dataSize;
	return UART_TransferSendNonBlocking(CMODULE_RMCALL_UART, &UART_BLE_handle, &UART_BLE_txTransfer);
}
mstatus_t RMCALL_HOST_Rx(void *_data, uint32_t _dataSize)
{
	UART_BLE_rxTransfer.data     = (uint8_t *)_data;
	UART_BLE_rxTransfer.dataSize = _dataSize;
	return UART_TransferReceiveNonBlocking(CMODULE_RMCALL_UART, &UART_BLE_handle, &UART_BLE_rxTransfer, NULL);

}
void RMCALL_HOST_TxAbort(void)
{
	UART_TransferAbortSend(CMODULE_RMCALL_UART, &UART_BLE_handle);
}
void RMCALL_HOST_RxAbort(void)
{
	UART_TransferAbortReceive(CMODULE_RMCALL_UART, &UART_BLE_handle);
}

void RMCALL_HOST_CpltCallback(UART_Type *base, uart_handle_t *handle, mstatus_t status, void *userData)
{
	(void)(userData);
	if (mStatus_UART_TxIdle == status)
    {
        RMCALL_TxIsr(&rmcall_host);
    }

    if (mStatus_UART_RxIdle == status)
    {
        RMCALL_RxIsr(&rmcall_host);
    }
}

#endif // CMODULE_USE_RMCALL
