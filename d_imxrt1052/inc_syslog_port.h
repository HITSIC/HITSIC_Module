#ifndef D_IMXRT1052_INC_SYSLOG_PORT_H
#define D_IMXRT1052_INC_SYSLOG_PORT_H

#include "cmodule_common.h"
#include <stdio.h>

#include "fsl_common.h"
#include "fsl_debug_console.h"

#ifdef DEBUG
#define SYSLOG_DEBUG
#endif

#define CMODULE_LOG_PRINTF(...)  (PRINTF(__VA_ARGS__))

#define CMODULE_LOG_LINEFEED ("\r\n")

#endif // ! D_IMXRT1052_INC_SYSLOG_PORT_H
