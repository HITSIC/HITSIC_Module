#include "sys_ltc.h"

#if (defined(CMODULE_USE_LTC) && (CMODULE_USE_LTC != 0U))

uint64_t LTC_GetTime_us(void)
{
    return COUNT_TO_USEC(PITMGR_GetLTC(), CMODULE_LTC_CLKFREQ);
}

uint64_t LTC_GetTime_ms(void)
{
    return COUNT_TO_MSEC(PITMGR_GetLTC(), CMODULE_LTC_CLKFREQ);
}

void LTC_Delay(uint64_t _t)
{
    uint64_t cnt = PITMGR_GetLTC() + _t;
    while (PITMGR_GetLTC() < cnt);
}

void LTC_Delay_us(uint64_t _t)
{
    uint64_t cnt = PITMGR_GetLTC() + USEC_TO_COUNT(_t, CMODULE_LTC_CLKFREQ);
    while (PITMGR_GetLTC() < cnt);
}

void LTC_Delay_ms(uint64_t _t)
{
    uint64_t cnt = PITMGR_GetLTC() + MSEC_TO_COUNT(_t, CMODULE_LTC_CLKFREQ);
    while (PITMGR_GetLTC() < cnt);
}

#endif // ! CMODULE_USE_LTC
