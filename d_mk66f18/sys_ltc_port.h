#ifndef D_MK66F18_SYS_LTC_PORT_H
#define D_MK66F18_SYS_LTC_PORT_H
#include <cmodule_common.h>

#if (defined(CMODULE_USE_LTC) && (CMODULE_USE_LTC != 0U))

#define CMODULE_LTC_CLKFREQ 60000000UL

/**
 * @brief : 获取LifeTimeCounter当前计时数。
 *
 *  PIT是减法计数器，这里获得的数值已经过转换，为单调递增。
 *
 * @return {uint64_t} : 返回LTC距离启动经过的时钟数
 */
static inline uint64_t PITMGR_GetTime(void)
{
    return ULLONG_MAX - PIT_GetLifetimeTimerCount(PIT);
}

#endif // ! CMODULE_USE_LTC

#endif // ! D_MK66F18_SYS_LTC_PORT_H
