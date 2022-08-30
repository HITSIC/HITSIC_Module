#include <sys_pitmgr.h>

#if defined(CMODULE_USE_PITMGR) && (CMODULE_USE_PITMGR > 0)

#ifdef __cplusplus
extern "C"{
#endif

#if defined(CMODULE_PITMGR_DEFAULT_IRQ) && (CMODULE_PITMGR_DEFAULT_IRQ > 0)
void PIT_IRQHandler(void)
{
    uint32_t flag;
    flag = PIT_GetStatusFlags(PIT, kPIT_Chnl_0);
    if(flag | kPIT_TimerFlag)
    {
        extern pitmgr_t pitmgr_main;
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
        PITMGR_Isr(&pitmgr_main);
    }
    
    __DSB();
    /* Added for, and affects, all PIT handlers. For CPU clock which is much larger than the IP bus clock,
     * CPU can run out of the interrupt handler before the interrupt flag being cleared, resulting in the
     * CPU's entering the handler again and again. Adding DSB can prevent the issue from happening.
     */
}
#endif // ! HTISIC_PITMGR_USE_IRQHANDLER

#ifdef __cplusplus
}
#endif

#endif // ! CMODULE_USE_PITMGR
