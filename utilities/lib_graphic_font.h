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
#ifndef UTILITIES_LIB_GRAPHIC_FONT_H
#define UTILITIES_LIB_GRAPHIC_FONT_H
#include "hitsic_common.h"

/*!
 * @addtogroup graphic_font
 * @{
 */

 /** @brief : 软件版本 */
#define LIB_GRAPHIC_FONT_VERSION (HITSIC_MAKE_VERSION(0U, 1U, 0U))

// begin is the first , endin is the one after the last.
extern const uint8_t graphic_asciiBegin;
extern const uint8_t graphic_asciiEndin;
extern const uint8_t graphic_asciiCnt;


/** ---------- FONT0816 ---------- **/

/*
 * this font is in Totally Hscan Mode.
 * each (uint8_t) represents ONE H line.
 * Total 16 (uint8_t)s stores ALL 16 H lines.
 */
extern const uint8_t graphic_font0816_newSongType[][16];


/** ---------- FONT0608 ---------- **/


/*
 * this font is in Totally Vscan Mode.
 * each (uint8_t) represents ONE V line.
 * Total 6 (uint8_t)s stores ALL 6 V lines.
 */
extern const uint8_t graphic_font0608_defaultType[][6];


/** ---------- FONT1215 ---------- **/

/*
 * this font is in Totally Hscan Mode.
 * every two (uint8_t) represents ONE H line(MSB).
 * Total 30 (uint8_t)s stores ALL 15 H lines.
 */
extern const uint8_t graphic_font1215_Consolas[][30];




/* @} */

#endif // ! UTILITIES_LIB_GRAPHIC_FONT_H
