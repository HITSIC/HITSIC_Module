#ifndef D_STM32F4XX_SYS_PITMGR_PORT_H
#define D_STM32F4XX_SYS_PITMGR_PORT_H

#include "hitsic_common.h"

#if defined(HITSIC_USE_PITMGR) && (HITSIC_USE_PITMGR > 0)

#define HITSIC_PITMGR_DEFAULT_IRQ 	(1U)

#endif // ! HITSIC_USE_PITMGR

#endif // ! D_STM32F4XX_SYS_PITMGR_PORT_H