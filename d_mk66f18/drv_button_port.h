#ifndef D_MK66F18_DRV_BUTTON_PORT_H
#define D_MK66F18_DRV_BUTTON_PORT_H
#include <sys_pitmgr.h>
#include "cmodule_common.h"
#include "sys_extint.h"

#if defined(CMODULE_USE_DRV_BUTTON) && (CMODULE_USE_DRV_BUTTON > 0)

#define BUTTON_TIME_SHRT 50u    //short press
#define BUTTON_SHRT_TOUT 200u   //short timeout
#define BUTTON_TIME_LONG 500u   //long press
#define BUTTON_TIME_LRPT 100u   //long press repeat, time calculated from key press. the actual time for repeat will be REPT-LONG
#define BUTTON_REPT_TOUT 550u   //repeat timeout
#define BUTTON_TIME_INTV 50u

#define BUTTON_PITMGR_TIME_MS 10

extern pitmgr_t pitmgr_main;
#define BUTTON_TIMER_MS     (pitmgr_main.tickCounter)

#endif // ! CMODULE_USE_DRV_BUTTON

#endif // ! D_MK66F18_DRV_BUTTON_PORT_H
