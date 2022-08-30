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

#ifndef D_MK24F12_TEXTMENU_PORT_H
#define D_MK24F12_TEXTMENU_PORT_H

#include <inc_stdlib.h>

#define mStatusGroup_MENU (205U)

/**
 * @name 调试输出
 * @ {
 */

#define TEXTMENU_USE_SYSLOG (1U)

#if defined(TEXTMENU_USE_SYSLOG) && (TEXTMENU_USE_SYSLOG > 0)

/*! 核心逻辑 LOG级别定义 */
#define TEXTMENU_MAIN_LOG_LVL    (3U)

/*! 数据存储 LOG级别定义 */
#define TEXTMENU_KVDB_LOG_LVL    (3U)

/*! 按键处理 LOG级别定义 */
#define TEXTMENU_BUTN_LOG_LVL    (3U)

/*! 菜单项目 LOG级别定义 */
#define TEXTMENU_ITEM_LOG_LVL    (3U)

/*! 菜单列表 LOG级别定义 */
#define TEXTMENU_LIST_LOG_LVL    (3U)

/*! 菜单迭代 LOG级别定义 */
#define TEXTMENU_ITER_LOG_LVL    (2U)

#else

#error Log output without SYSLOG is not supported yet!
 // TODO
#endif // ! TEXTMENU_USE_SYSLOG

/* @ } */

/** @brief : 根菜单最大容量 */
#define TEXTMENU_ROOT_SIZE 35 //maximum size of Root Menu

/**
 * @name 事件任务接口
 * @brief : 菜单消息调用事件处理线程。
 *
 * 对于裸机，可以使用一个空闲的中断向量来模拟线程，但需要注意优先级不可过高。
 * 对于RTOS，可以直接使用一个线程。
 * @ {
 */
#include <MK24F12.h>
#include "fsl_common.h"
#define TEXTMENU_SERVICE_IRQHandler (Reserved71_IRQHandler)  ///< 要使用的中断服务函数
#define TEXTMENU_SERVICE_IRQn (Reserved71_IRQn)              ///< 要使用的中断号
#define TEXTMENU_SERVICE_IRQPrio (12u)                       ///< 中断优先级，需要设置一个较低的值，以免打断重要任务。

#define TEXTMENU_SERVICE_SEM_GIVE() NVIC_SetPendingIRQ(TEXTMENU_SERVICE_IRQn)
#define TEXTMENU_SERVICE_SEM_TAKE() NVIC_ClearPendingIRQ(TEXTMENU_SERVICE_IRQn)
/* @ } */



/**
 * @name 屏幕打印接口
 * @ {
 */


/**
 * @brief 字符缓存大小（行、列）
 * 
 * 行、列均为实际能够显示的行数和列数，不需要考虑C语言的'\0'。
 */
#define TEXTMENU_DISPLAY_STRBUF_ROW (16u)
#define TEXTMENU_DISPLAY_STRBUF_COL (21u)

/**
 * @brief 启用色盘
 */
#define TEXTMENU_USE_PALETTE (0U)

#if defined(TEXTMENU_USE_PALETTE) && (TEXTMENU_USE_PALETTE > 0)

#define TEXTMENU_DISPLAY_PAL_IDX_NORMAL_F (1U) ///< 前景色-正常
#define TEXTMENU_DISPLAY_PAL_IDX_NORMAL_B (0U) ///< 背景色-正常
#define TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_F (1U) ///< 前景色-高亮
#define TEXTMENU_DISPLAY_PAL_IDX_HLIGHT_B (0U) ///< 背景色-高亮
#define TEXTMENU_DISPLAY_PAL_IDX_TITBAR_F (1U) ///< 前景色-标题
#define TEXTMENU_DISPLAY_PAL_IDX_TITBAR_B (0U) ///< 背景色-标题

#endif // ! TEXTMENU_USE_PALETTE


/* @ } */

/**
 *  @name 键值数据库
 *  @ {
 */
/*! @brief 是否启用非易失性键值数据库存储支持。 */
#define TEXTMENU_USE_KVDB (0U)

// #if defined(TEXTMENU_USE_KVDB) && (TEXTMENU_USE_KVDB > 0)

/**
 * @brief : 局部存储 Region Storage
 */
#define TEXTMENU_NVM_REGION_CNT (3) 					///< 局部存储区的数量

// #endif // ! TEXTMENU_USE_KVDB

/* @ } */

/**
 *  @name 键值数据库
 *  @ {
 */
/*! @brief 是否启用非易失性键值数据库存储支持。 */
#define TEXTMENU_USE_NVM (1U)

#if defined(TEXTMENU_USE_NVM) && (TEXTMENU_USE_NVM > 0)

#include "drv_ftfx_flash.h"

#include "textmenu_keyop.h"

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
#define TEXTMENU_NVM_SECTOR_SIZE (0u)
/**
 * @brief : 全局存储 Global Storage
 */
#define TEXTMENU_NVM_GLOBAL_SECT_SIZE (2u)	///< 全局存储区占用的扇区数
#define TEXTMENU_NVM_GLOBAL_SECT_OFFSET (2u) ///< 全局存储区扇区偏移
/**
 * @brief : 局部存储 Region Storage
 */
#define TEXTMENU_NVM_REGION_CNT (3) 					///< 局部存储区的数量
#define TEXTMENU_NVM_REGION_SECT_SIZE  (4u)				///< 每个局部存储区占用的扇区数

#define TEXTMENU_NVM_AddressRead(addr, buf, byteCnt)		FLASH_AddressRead(addr, buf, byteCnt)	///< 读指定地址。必须返回表示操作是否成功的值。
#define TEXTMENU_NVM_SectorRead(sect, buf)		FLASH_SectorRead(sect, buf)						///< 读指定扇区。sect为扇区号，buf为缓存区。必须返回表示操作是否成功的值。
#define TEXTMENU_NVM_SectorWrite(sect, buf)		FLASH_SectorWrite(sect, buf)					///< 写指定扇区。sect为扇区号，buf为缓存区。必须返回表示操作是否成功的值。
#define TEXTMENU_NVM_RETVAL_SUCCESS				mstatus_FTFx_Success							///< flash接口操作成功的返回值。如果返回值不等一此值则表示操作失败，MENU_NVM接口将向上层报告错误（mstatus_Fail）。

extern int32_t menu_currRegionNumAdj[3];
extern const char menu_itemNameStr_RegnSel[];

/**
 * @brief : 保存整个菜单到NVM。
 *
 * @param  {int32_t} _region :  所选择的局部存储区。
 */
void MENU_Data_NvmSave(int32_t _region);

/**
 * @brief : 保存整个菜单到NVM。
 * 该函数将使用全局变量 menu_currRegionNum 中保存的局部存储区号。
 * 
 * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
 */
void MENU_Data_NvmSave_Boxed(menu_keyOp_t *const _op);

/**
 * @brief : 从NVM读取整个菜单。
 *
 * @param  {int32_t} _region : 所选择的局部存储区。
 */
void MENU_Data_NvmRead(int32_t _region);

/**
 * @brief : 从NVM读取整个菜单。
 * 该函数将使用全局变量 menu_currRegionNum 中保存的局部存储区号。
 *
 * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
 */
void MENU_Data_NvmRead_Boxed(menu_keyOp_t *const _op);

/**
 * @brief : 保存当前局部存储区号到NVM。
 * 该数值设置为不自动保存。
 *
 */
void MENU_Data_NvmSaveRegionConfig(void);

/**
 * @brief : 保存当前局部存储区号到NVM。
 * 该数值设置为不自动保存。
 *
 * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
 */
void MENU_Data_NvmSaveRegionConfig_Boxed(menu_keyOp_t *const _op);

/**
 * @brief : 从NVM中读取当前局部存储区号。
 * 该数值设置为不自动保存。
 *
 */
void MENU_Data_NvmReadRegionConfig(void);

/**
 * @brief : 从NVM中读取当前局部存储区号。
 * 该数值设置为不自动保存。
 * 
 * @param {menu_keyOp_t* const} _op : 按键操作接口传入的按键操作
 */
void MENU_Data_NvmReadRegionConfig_Boxed(menu_keyOp_t *const _op);

#endif // ! TEXTMENU_USE_NVM

/* @ } */


#endif // ! D_MK24F12_TEXTMENU_PORT_H

