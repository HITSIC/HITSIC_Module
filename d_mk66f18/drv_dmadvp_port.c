#include <drv_dmadvp_port.h>

#if defined(CMODULE_USE_DMADVP) && (CMODULE_USE_DMADVP > 0)
#include "sys_extint.h"

/*!
 * @addtogroup dmadvp
 * @{
 */

DMADVP_Type __DMADVP0 =
{
    .vsnc_intc = RTEPIN_DIGITAL_CAM_VSNC_PORT,
    .vsnc_pin = RTEPIN_DIGITAL_CAM_VSNC_PIN,
    .pclk_intc = RTEPIN_DIGITAL_CAM_PCLK_PORT,
    .pclk_pin = RTEPIN_DIGITAL_CAM_PCLK_PIN,

    .dmaDataAddress = DMADVP0_DATA_DMA_ADDR,
    .dmaChannel = 0U,
    .dmauxChannel = 0U,
    .dmamuxRequestSrc = kDmaRequestMux0PortC,
    .dmaIrqn = DMA0_DMA16_IRQn,
    .dmaIrqPrio = 8U,

    .imgSize = 0U,

    .vsncInterruptCfg = kPORT_InterruptRisingEdge,
    .pclkInterruptCfg = kPORT_DMARisingEdge,

    .extintHandle =
    {
        .index = RTEPIN_DIGITAL_CAM_VSNC_PIN,
        .handler = NULL,
        .userData = NULL,
    },
};

/* @} */

#endif // ! CMODULE_USE_DMADVP

