#ifndef D_IMXRT1052_SYS_PITMGR_PORT_H
#define D_IMXRT1052_SYS_PITMGR_PORT_H

#include "cmodule_common.h"

#if defined(CMODULE_USE_PITMGR) && (CMODULE_USE_PITMGR > 0)

#include "fsl_common.h"

#define CMODULE_PITMGR_DEFAULT_IRQ 	(0U)

#endif // ! CMODULE_USE_PITMGR

#endif // ! D_IMXRT1052_SYS_PITMGR_PORT_H
