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
/**
 * @file 	:	sys_rmcall.hpp
 * @author  :	Chekhov Ma
 * @version :	v1.0-beta.0
 *
 * @date 	:	v0.1-beta.0 2020.08.08
 * @date    :   v1.0-beta.0 2021.06.25
 *
 * @brief   :   远程调用组件
 * 
 */

#ifndef SYS_RMCALL_H
#define SYS_RMCALL_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "cmodule_common.h"

#if defined(CMODULE_USE_RMCALL) && (CMODULE_USE_RMCALL != 0U)
#include <sys_rmcall_port.h>

#if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
#include "lib_crc32.h"
#endif // RMCALL_TRAILER_CRC32

#include <m-dict.h>

 /*!
  * @addtogroup rmcall
  * @{
  */

/** @brief : 软件版本 */
#define SYS_RMCALL_VERSION (CMODULE_MAKE_VERSION(1U, 0U, 0U))


/*! @brief Error codes for the RMCALL driver. */
enum
{
    mStatus_RMCALL_TxBusy = CMODULE_MAKE_STATUS(mStatusGroup_RMCALL, 0),
    mStatus_RMCALL_TxError  = CMODULE_MAKE_STATUS(mStatusGroup_RMCALL, 1),
    mStatus_RMCALL_RxBusy = CMODULE_MAKE_STATUS(mStatusGroup_RMCALL, 2),
    mStatus_RMCALL_RxError = CMODULE_MAKE_STATUS(mStatusGroup_RMCALL, 3),
#if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
    mStatus_RMCALL_ParityError = CMODULE_MAKE_STATUS(mStatusGroup_RMCALL, 3), /*!< CRC check failed. */
#endif // RMCALL_TRAILER_CRC32
    
};

typedef void (*rmcall_handler_t)(void *_recvData, uint16_t _recvSize, void *_userData);

typedef struct _rmcall_handle
{
    uint16_t handleId;
    rmcall_handler_t handler;
    void *userData;
}rmcall_handle_t;

__PACKED struct _rmcall_header
{
    uint32_t magic;
    uint16_t handleId;
    uint16_t dataSize;
};

typedef struct _rmcall_header rmcall_header_t;

enum rmcall_statusFlag_t
{
    rmcall_statusFlag_txHead = 1U << 0U,
    rmcall_statusFlag_txData = 2U << 0U,
    rmcall_statusFlag_txBusy = 3U << 0U,
    rmcall_statusFlag_rxHead = 1U << 2U,
    rmcall_statusFlag_rxData = 2U << 2U,
    rmcall_statusFlag_rxBusy = 3U << 2U,

    rmcall_statusFlag_rxDataDiscard = 1U << 9U,
};


// enum rmcall_statusFlag_t
// {
//     rmcall_statusFlag_txHead = 1U << 0U,
//     rmcall_statusFlag_txData = 1U << 1U,
// #if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
//     rmcall_statusFlag_txTail = 1U << 2U,
// #endif // RMCALL_TRAILER_CRC32
//     rmcall_statusFlag_txBusy = 15U << 0U,
//     rmcall_statusFlag_rxHead = 1U << 4U,
//     rmcall_statusFlag_rxData = 1U << 5U,
//     #if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
//     rmcall_statusFlag_rxTail = 1U << 6U,
// #endif // RMCALL_TRAILER_CRC32
//     rmcall_statusFlag_rxBusy = 15U << 4U,

//     rmcall_statusFlag_rxDataDiscard = 1U << 9U,
// };

typedef mstatus_t(*rmcall_transfer_t)(void *_data, uint32_t dataSize);
typedef void(*rmcall_transferAbort_t)(void);

typedef struct 
{
    mstatus_t(*xfer_tx)(void *_data, uint32_t dataSize);
    mstatus_t(*xfer_rx)(void *_data, uint32_t dataSize);
    void(*xferAbort_tx)(void);
    void(*xferAbort_rx)(void);
    //rmcall_transfer_t xfer_tx, xfer_rx;
    //rmcall_transferAbort_t xferAbort_tx, xferAbort_rx;
}rmcall_teleport_t;

typedef struct _rmcall_config
{
    rmcall_teleport_t *teleport;//TODO: const
}rmcall_config_t;
    

static inline bool oor_equal_p(uint16_t k, unsigned char n) {
  return k == 65535 - n;//FIXME
}
static inline void oor_set(uint16_t *k, unsigned char n) {
  *k = 65535 - n;//FIXME
}

#ifdef __cplusplus
extern "C" {
#endif
DICT_OA_DEF2(rmcall_isrDict, uint16_t, M_OPEXTEND(M_DEFAULT_OPLIST, OOR_EQUAL(oor_equal_p), OOR_SET(oor_set M_IPTR)) , rmcall_handle_t*, M_PTR_OPLIST)
#ifdef __cplusplus
}
#endif

typedef struct _rmcall
{
    uint32_t statusFlag;

    //rmcall_transfer_t xfer_tx, xfer_rx;
    //rmcall_transferAbort_t xferAbort_tx, xferAbort_rx;
    rmcall_teleport_t *teleport; //TODO: const

    rmcall_header_t txHeaderBuffer;
    void *txDataBuffer;
#if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
    uint32_t txTailBuffer;
#endif // RMCALL_TRAILER_CRC32
    rmcall_header_t rxHeaderBuffer;
    void *rxDataBuffer;
#if defined(RMCALL_TRAILER_CRC32) && (RMCALL_TRAILER_CRC32 != 0U)
    uint32_t rxTailBuffer;
#endif // RMCALL_TRAILER_CRC32

    rmcall_isrDict_t isrDict;

}rmcall_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief : RMCALL初始化。
 *
 * @return {mstatus_t} : 成功返回mStatus_Success，异常返回mStatus_Fail。
 */
mstatus_t RMCALL_Init(rmcall_t *_inst, rmcall_config_t const * const _config);

void RMCALL_DeInit(rmcall_t *_inst);

/**
 * @brief : 向RMCALL中断表末尾插入一个新的任务描述符。
 *  该函数仅做数据检查并赋值。需要互斥保护。
 *
 * @param {rmcall_t*} _inst          : 要操作的RMCALL实例。
 * @param {rmcall_handle_t*} _handle : 该RMCALL任务的任务描述符指针。
 * @return {mstatus_t}                : 成功返回mStatus_Success，异常返回mStatus_Fail。
 */
mstatus_t RMCALL_HandleInsert(rmcall_t *_inst, rmcall_handle_t *_handle);

/**
 * @brief : 从RMCALL中断表中移除一个任务描述符。
 *
 * @param {rmcall_t*} _inst          : 要操作的RMCALL实例。
 * @param {rmcall_handle_t*} _handle : 该RMCALL任务的任务描述符指针。
 * @return {mstatus_t}                : 成功返回mStatus_Success，异常返回mStatus_Fail。
 */
mstatus_t RMCALL_HandleRemove(rmcall_t *_inst, rmcall_handle_t *_handle);


/**
 * @brief : 向远端发送RMCALL命令。
 *
 * @param {rmcall_t*} _inst         : 要操作的RMCALL实例。
 * @param {uint8_t} _handleId       : 要发送的命令ID。
 * @param {void*} _data             : 要发送的数据，无数据填NULL。
 * @param {uint16_t} dataSize       : 要发送的数据长度，无数据填0。
 */
mstatus_t RMCALL_CommandSend(rmcall_t *_inst, uint16_t _handleId, void *_data, uint16_t _dataSize);

/**
 * @brief : RMCALL接收使能。
 *
 * @param {rmcall_t*} _inst         : 要操作的RMCALL实例。
 * @return {mstatus_t} : 成功返回mStatus_Success，异常返回mStatus_Fail。
 */
mstatus_t RMCALL_CommandRecvEnable(rmcall_t *_inst);

/**
 * @brief : RMCALL接收失能。
 *
 * @param {rmcall_t*} _inst         : 要操作的RMCALL实例。
 * @return {mstatus_t} : 成功返回mStatus_Success，异常返回mStatus_Fail。
 */
mstatus_t RMCALL_CommandRecvDisable(rmcall_t *_inst);

/**
 * @brief : RMCALL中断的处理函数。被IRQHandler调用。
 */
void RMCALL_TxIsr(rmcall_t *_inst);
void RMCALL_RxIsr(rmcall_t *_inst);

#ifdef __cplusplus
}
#endif

/* @} */

#endif // CMODULE_USE_RMCALL

#endif // ! SYS_RMCALL_H
