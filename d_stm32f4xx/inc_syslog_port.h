#ifndef D_STM32F4XX_INC_SYSLOG_PORT_H
#define D_STM32F4XX_INC_SYSLOG_PORT_H

#include "cmodule_common.h"
#include "stdio.h"

#ifdef DEBUG
#define SYSLOG_DEBUG
#endif

#define CMODULE_LOG_PRINTF(...)  (printf(__VA_ARGS__))

#define CMODULE_LOG_LINEFEED ("\r\n")

#endif // ! D_STM32F4XX_INC_SYSLOG_PORT_H
