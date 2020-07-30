/**
 * Copyright 2018 - 2019 HITSIC
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

#ifndef _HITSIC_COMMON_H_
#define _HITSIC_COMMON_H_

#include "inc_fsl_kv10z7.h"
#include "stdint.h"

#ifndef CPU_MKV10Z128VLC7
#error CPU incorrect !
#endif // ! CPU_MKV10Z128VLC7

/**
 * @brief : 软件版本产生方式定义
 */
#define HITSIC_MAKE_VERSION(major, minor, patch) (((major) << 16) | ((minor) << 8) | (patch))




typedef I2C_Type HAL_I2C_Type;
typedef SPI_Type HAL_SPI_Type;
typedef UART_Type HAL_UART_Type;




#ifdef __cplusplus
extern "C"{
#endif


void HAL_EnterCritical(void);

void HAL_ExitCritical(void);



//I2C MEM Wrapper

status_t HAL_I2C_Mem_ReadBlocking(HAL_I2C_Type *_i2c, uint8_t _addr, uint32_t _reg, uint8_t _regSize, uint8_t* _data, uint32_t _dataSize);
status_t HAL_I2C_Mem_WriteBlocking(HAL_I2C_Type *_i2c, uint8_t _addr, uint32_t _reg, uint8_t _regSize, uint8_t* _data, uint32_t _dataSize);


#ifdef __cplusplus
}
#endif













    


#endif // ! _HITSIC_COMMON_H_