/**
 * Copyright 2018 - 2020 HITSIC
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

#ifndef D_MK66F18_APP_MENU_PORT_H
#define D_MK66F18_APP_MENU_PORT_H

#include <app_menu_def.h>
#include <inc_stdlib.h>
#include "hitsic_common.h"

#if defined(HITSIC_USE_APP_MENU) && (HITSIC_USE_APP_MENU > 0)

/**
 * @name 调试输出
 * @ {
 */

/*! 核心逻辑 LOG级别定义 */
#define HITSIC_MENU_MAIN_LOG_LVL    (3U)

/*! 数据存储 LOG级别定义 */
#define HITSIC_MENU_KVDB_LOG_LVL    (3U)

/*! 按键处理 LOG级别定义 */
#define HITSIC_MENU_BUTN_LOG_LVL    (3U)

/*! 菜单项目 LOG级别定义 */
#define HITSIC_MENU_ITEM_LOG_LVL    (3U)

/*! 菜单列表 LOG级别定义 */
#define HITSIC_MENU_LIST_LOG_LVL    (3U)

/*! 菜单迭代 LOG级别定义 */
#define HITSIC_MENU_ITER_LOG_LVL    (2U)

/* @ } */

/** @brief : 根菜单最大容量 */
#define HITSIC_MENU_ROOT_SIZE 35 //maxium size of Root Menu

/**
 * @name 事件任务接口
 * @brief : 菜单消息调用所使用的中断信息。
 * 可以使用任何当前工程中未使用的中断。注意中断号和中断服务函数
 * 必须对应。优先级不可过高。
 * @ {
 */
#define HITSIC_MENU_SERVICE_IRQHandler (Reserved85_IRQHandler)  ///< 要使用的中断服务函数
#define HITSIC_MENU_SERVICE_IRQn (Reserved85_IRQn)              ///< 要使用的中断号
#define HITSIC_MENU_SERVICE_IRQPrio (12u)                       ///< 中断优先级，需要设置一个较低的值，以免打断重要任务。

#define HITSIC_MENU_SERVICE_SEM_GIVE() NVIC_SetPendingIRQ(HITSIC_MENU_SERVICE_IRQn)
#define HITSIC_MENU_SERVICE_SEM_TAKE() NVIC_ClearPendingIRQ(HITSIC_MENU_SERVICE_IRQn)
/* @ } */

/**
 * @name 按键输入接口
 * @brief : 是否使用菜单自带的按键事件管理
 * @ {
 */
#define HITSIC_MENU_USE_BUTTON (1U)


#if defined(HITSIC_MENU_USE_BUTTON) && (HITSIC_MENU_USE_BUTTON > 0)
#include <drv_button.h>
/** @brief : 菜单使用的五向按键初始化。每组数据前两个是GPIO和Pin，其余数据为0。 */
#define HITSIC_MENU_BUTTON_5DIR_BSP_INIT  \
    {                                     \
        {                                 \
            RTEPIN_DIGITAL_BUTTON_OK_GPIO, \
            RTEPIN_DIGITAL_BUTTON_OK_PIN,   \
            kPORT_InterruptOrDMADisabled, \
            0,                            \
            BUTTON_STAT_NONE,             \
            NULL,                         \
        },                                \
        {                                 \
        	RTEPIN_DIGITAL_BUTTON_UP_GPIO,  \
			RTEPIN_DIGITAL_BUTTON_UP_PIN,   \
            kPORT_InterruptOrDMADisabled, \
            0,                            \
            BUTTON_STAT_NONE,             \
            NULL,                         \
        },                                \
        {                                 \
        	RTEPIN_DIGITAL_BUTTON_DN_GPIO,  \
			RTEPIN_DIGITAL_BUTTON_DN_PIN,   \
            kPORT_InterruptOrDMADisabled, \
            0,                            \
            BUTTON_STAT_NONE,             \
            NULL,                         \
        },                                \
        {                                 \
        	RTEPIN_DIGITAL_BUTTON_LF_GPIO,  \
			RTEPIN_DIGITAL_BUTTON_LF_PIN,   \
            kPORT_InterruptOrDMADisabled, \
            0,                            \
            BUTTON_STAT_NONE,             \
            NULL,                         \
        },                                \
        {                                 \
        	RTEPIN_DIGITAL_BUTTON_RT_GPIO,  \
			RTEPIN_DIGITAL_BUTTON_RT_PIN,   \
            kPORT_InterruptOrDMADisabled, \
            0,                            \
            BUTTON_STAT_NONE,             \
            NULL,                         \
        },                                \
    }

#endif // ! HITSIC_MENU_USE_BUTTON

/* @ } */

/**
 * @brief 启用帧缓存接口
 */
#define HITSIC_MENU_USE_FRAME_BUFFER (1U)

/**
 * @brief 屏幕打印接口
 * @ {
 */
#if defined(HITSIC_MENU_USE_FRAME_BUFFER) && (HITSIC_MENU_USE_FRAME_BUFFER > 0)
#include <drv_disp_ssd1327.h>
#include <lib_graphic.h>
#define HITSIC_MENU_DISPLAY_BUFFER_CLEAR() (MENU_FrameBufferClear())
#define HITSIC_MENU_DISPLAY_PRINT(row, col, str) (MENU_FrameBufferPrint(row, col, str))
#define HITSIC_MENU_DISPLAY_BUFFER_UPDATE() (MENU_FrameBufferUpdate())

/**
 * @brief 清除缓存区。
 */
void MENU_FrameBufferClear(void);

/**
 * @brief 向缓存区打印字符串。
 */
void MENU_FrameBufferPrint(uint16_t x, uint16_t y, char *str);

/**
 * @brief 将缓存区上传到屏幕。
 */
void MENU_FrameBufferUpdate(void);


#else // HITSIC_MENU_USE_FRAME_BUFFER
#define HITSIC_MENU_DISPLAY_BUFFER_CLEAR() (0)
#define HITSIC_MENU_DISPLAY_PRINT(row, col, str) (DISP_SSD1306_Print_F6x8(row, col, str))
#define HITSIC_MENU_DISPLAY_BUFFER_UPDATE() (0)
#endif // ! HITSIC_MENU_USE_FRAME_BUFFER

/* @ } */

/**
 *  @name 非易失性存储
 *  @ {
 */
/*! @brief 是否启用非易失性存储支持。目前仅支持块级存储接口。将于未来添加文件存储接口 */
#define HITSIC_MENU_USE_NVM (1U)


#if defined(HITSIC_MENU_USE_NVM) && (HITSIC_MENU_USE_NVM > 0)

#include <drv_ftfx_flash.h>

/**
 * ********** NVM存储变量定义 **********
 */

//gl = global
//rg = region
//addr = address
//sect = sector
/**
 * @brief : 每个扇区包含的字节数
 */
#define HITSIC_MENU_NVM_SECTOR_SIZE (flash_sectorSize)
/**
 * @brief : 全局存储 Global Storage
 */
#define HITSIC_MENU_NVM_GLOBAL_SECT_SIZE (2u)	///< 全局存储区占用的扇区数
#define HITSIC_MENU_NVM_GLOBAL_SECT_OFFSET (2u) ///< 全局存储区扇区偏移
/**
 * @brief : 局部存储 Region Storage
 */
#define HITSIC_MENU_NVM_REGION_CNT (3) 					///< 局部存储区的数量
#define HITSIC_MENU_NVM_REGION_SECT_SIZE  (4u)				///< 每个局部存储区占用的扇区数

#define HITSIC_MENU_NVM_AddressRead(addr, buf, byteCnt)		FLASH_AddressRead(addr, buf, byteCnt)	///< 读指定地址。必须返回表示操作是否成功的值。
#define HITSIC_MENU_NVM_SectorRead(sect, buf)		FLASH_SectorRead(sect, buf)						///< 读指定扇区。sect为扇区号，buf为缓存区。必须返回表示操作是否成功的值。
#define HITSIC_MENU_NVM_SectorWrite(sect, buf)		FLASH_SectorWrite(sect, buf)					///< 写指定扇区。sect为扇区号，buf为缓存区。必须返回表示操作是否成功的值。
#define HITSIC_MENU_NVM_RETVAL_SUCCESS				kStatus_FTFx_Success							///< flash接口操作成功的返回值。如果返回值不等一此值则表示操作失败，MENU_NVM接口将向上层报告错误（kStatus_Fail）。

#endif // ! HITSIC_MENU_USE_NVM

/* @ } */

#endif // ! HITSIC_USE_APP_MENU


#endif // ! D_MK66F18_APP_MENU_PORT_H

