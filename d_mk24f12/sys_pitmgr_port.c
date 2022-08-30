#include <sys_pitmgr.h>

#if defined(CMODULE_PITMGR_DEFAULT_IRQ) && (CMODULE_PITMGR_DEFAULT_IRQ > 0)

void LPTMR0_IRQHandler(void)
{
    extern pitmgr_t pitmgr_main;
	LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
	PITMGR_Isr(&pitmgr_main);
}

#endif // ! HTISIC_PITMGR_USE_IRQHANDLER
