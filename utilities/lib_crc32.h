#ifndef UTILITIES_LIB_CRC32_H
#define UTILITIES_LIB_CRC32_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*!
 * @addtogroup pidctrl
 * @{
 */

/** @brief : 软件版本 */
//#define LIB_CRC32_VERSION (HITSIC_MAKE_VERSION(0U, 1U, 0U))

/**
 * @brief Compute the 32-bit CRC.
 * 
 * This CRC can be specified as:
 * Width  : 32
 * Poly   : 0x04c11db7
 * Init   : parameter, typically 0xffffffff
 * RefIn  : false
 * RefOut : false
 * XorOut : 0
 * This differs from the "standard" CRC-32 algorithm in that the values
 * are not reflected, and there is no final XOR value.  These differences
 * make it easy to compose the values of multiple blocks.
 * 
 * @param buf Data buffer.
 * @param len Length in bytes of the data buffer.
 * @param init The starting value.
 * @return uint32_t CRC result.
 */
uint32_t CRC32_Calculate (void const *data, int32_t len, uint32_t init);

/*! @} */


#endif // UTILITIES_LIB_CRC32_H