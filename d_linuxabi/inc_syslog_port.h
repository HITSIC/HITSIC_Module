#ifndef D_LINUXABI_INC_SYSLOG_PORT_H
#define D_LINUXABI_INC_SYSLOG_PORT_H

#include "cmodule_common.h"
#include <stdio.h>

#if defined(DEBUG) && (DEBUG != 0U) // ENABLE DEBUG
#define SYSLOG_DEBUG
#endif

#define CMODULE_LOG_PRINTF(...)  (printf(__VA_ARGS__))

#define CMODULE_LOG_LINEFEED ("\n")

#endif // ! D_LINUXABI_INC_SYSLOG_PORT_H
