/**
 * Copyright 2018 - 2021 HITSIC
 * All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef D_DEFAULT_CMODULE_COMMON_H
#define D_DEFAULT_CMODULE_COMMON_H

#include <inc_mstatus.h>
#include <stdint.h>


/** DRV */

// #ifndef CMODULE_USE_FTFX_FLASH
// #define CMODULE_USE_FTFX_FLASH (0U)
// #endif // ! CMODULE_USE_FTFX_FLASH

// #ifndef CMODULE_USE_DISP_SSD1306
// #define CMODULE_USE_DISP_SSD1306 (0U)
// #endif // ! CMODULE_USE_DISP_SSD1306

// #ifndef CMODULE_USE_DISP_SSD1327
// #define CMODULE_USE_DISP_SSD1327 (0U)
// #endif // ! CMODULE_USE_DISP_SSD1327

// #ifndef CMODULE_USE_DISP_ST7789
// #define CMODULE_USE_DISP_ST7789 (0U)
// #endif // ! CMODULE_USE_DISP_ST7789

// #ifndef CMODULE_USE_DRV_BUTTON
// #define CMODULE_USE_DRV_BUTTON (0U)
// #endif // ! CMODULE_USE_DRV_BUTTON

// #ifndef CMODULE_USE_DMADVP
// #define CMODULE_USE_DMADVP (0U)
// #endif // ! CMODULE_USE_DMADVP

// #ifndef CMODULE_USE_CAM_ZF9V034
// #define CMODULE_USE_CAM_ZF9V034 (0U)
// #endif // ! CMODULE_USE_CAM_ZF9V034

/** SYS */

// #ifndef CMODULE_USE_LTC
// #define CMODULE_USE_LTC       (0U)
// #endif // ! CMODULE_USE_LTC

#ifndef CMODULE_USE_PITMGR
#define CMODULE_USE_PITMGR 		(1U)
#endif // ! CMODULE_USE_PITMGR

// #ifndef CMODULE_USE_EXTINT
// #define CMODULE_USE_EXTINT 		(0U)
// #endif // ! CMODULE_USE_EXTINT

#ifndef CMODULE_USE_RMCALL
#define CMODULE_USE_RMCALL 		(1U)
#endif // ! CMODULE_USE_RMCALL

/** APP */

// #ifndef CMODULE_USE_APP_SVBMP
// #define CMODULE_USE_APP_SVBMP (1U)
// #endif // ! CMODULE_USE_APP_SVBMP

#ifdef __cplusplus
extern "C"{
#endif

void HAL_EnterCritical(void);

void HAL_ExitCritical(void);
    
#ifdef __cplusplus
}
#endif

#endif // ! D_DEFAULT_CMODULE_COMMON_H
