#ifndef D_LINUXABI_SYS_RMCALL_PORT_H
#define D_LINUXABI_SYS_RMCALL_PORT_H
#include "cmodule_common.h"

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#if defined(CMODULE_USE_RMCALL) && (CMODULE_USE_RMCALL > 0)

#define RMCALL_HEADER_MAGIC (0x554768A0U)
#define RMCALL_SYSLOG_LVL (3U)
#define RMCALL_PUBLIC_BUF_SIZE (256U)
#define mStatusGroup_RMCALL (201U)

#define RMCALL_TRAILER_CRC32 (1U)

// TODO: implement this
#define RMCALL_USE_GENERIC_HANDLE (0U) // a generic handle, called when no maching id or parity error.

#ifdef __cplusplus
extern "C" {
#endif

mstatus_t RMCALL_HOST_Tx(void *_data, uint32_t _dataSize);
mstatus_t RMCALL_HOST_Rx(void *_data, uint32_t _dataSize);
void RMCALL_HOST_TxAbort(void);
void RMCALL_HOST_RxAbort(void);

#ifdef __cplusplus
}
#endif

#endif // CMODULE_USE_RMCALL

#endif // ! D_LINUXABI_SYS_RMCALL_PORT_H
