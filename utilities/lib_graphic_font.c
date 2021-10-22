#include "lib_graphic_font.h"

/*!
 * @addtogroup graphic_font
 * @{
 */
#if 0
// begin is the first , endin is the one after the last.
const uint8_t graphic_asciiBegin = 32;
const uint8_t graphic_asciiEndin = 127;
const uint8_t graphic_asciiCnt = 95;

#else // __cplusplus

#endif // ! __cplusplus


/** ---------- FONT0816 ---------- **/

/*
 * this font is in Totally Hscan Mode.
 * each (uint8_t) represents ONE H line.
 * Total 16 (uint8_t)s stores ALL 16 H lines.
 */

const uint8_t graphic_font0816_newSongType[graphic_asciiCnt][16] =
{
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/
{0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x08,0x08,0x00,0x00},/*"!",1*/
{0x00,0x48,0x24,0x24,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*""",2*/
{0x00,0x00,0x00,0x48,0x48,0x48,0x7E,0x24,0x24,0x24,0x7E,0x24,0x24,0x24,0x00,0x00},/*"#",3*/
{0x00,0x00,0x10,0x3C,0x52,0x52,0x12,0x1C,0x30,0x50,0x50,0x52,0x52,0x3C,0x10,0x10},/*"$",4*/
{0x00,0x00,0x00,0x22,0x25,0x15,0x15,0x0D,0x2A,0x58,0x54,0x54,0x52,0x22,0x00,0x00},/*"%",5*/
{0x00,0x00,0x00,0x0C,0x12,0x12,0x12,0x0A,0x76,0x25,0x29,0x19,0x91,0x6E,0x00,0x00},/*"&",6*/
{0x00,0x06,0x04,0x04,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"'",7*/
{0x00,0x40,0x20,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x10,0x20,0x40,0x00},/*"(",8*/
{0x00,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x08,0x08,0x04,0x02,0x00},/*")",9*/
{0x00,0x00,0x00,0x00,0x08,0x08,0x6B,0x1C,0x1C,0x6B,0x08,0x08,0x00,0x00,0x00,0x00},/*"*",10*/
{0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x10,0xFE,0x10,0x10,0x10,0x00,0x00,0x00,0x00},/*"+",11*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x04,0x04,0x02},/*",",12*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"-",13*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x00,0x00},/*".",14*/
{0x00,0x00,0x40,0x20,0x20,0x20,0x10,0x10,0x08,0x08,0x08,0x04,0x04,0x02,0x02,0x00},/*"/",15*/
{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"0",16*/
{0x00,0x00,0x00,0x10,0x1C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"1",17*/
{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x40,0x20,0x10,0x08,0x04,0x42,0x7E,0x00,0x00},/*"2",18*/
{0x00,0x00,0x00,0x3C,0x42,0x42,0x40,0x20,0x18,0x20,0x40,0x42,0x42,0x3C,0x00,0x00},/*"3",19*/
{0x00,0x00,0x00,0x20,0x30,0x30,0x28,0x24,0x24,0x22,0xFE,0x20,0x20,0xF8,0x00,0x00},/*"4",20*/
{0x00,0x00,0x00,0x7E,0x02,0x02,0x02,0x1E,0x22,0x40,0x40,0x42,0x22,0x1C,0x00,0x00},/*"5",21*/
{0x00,0x00,0x00,0x18,0x24,0x02,0x02,0x3A,0x46,0x42,0x42,0x42,0x44,0x38,0x00,0x00},/*"6",22*/
{0x00,0x00,0x00,0x7E,0x42,0x20,0x20,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x00,0x00},/*"7",23*/
{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00},/*"8",24*/
{0x00,0x00,0x00,0x1C,0x22,0x42,0x42,0x42,0x62,0x5C,0x40,0x40,0x24,0x18,0x00,0x00},/*"9",25*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00},/*":",26*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x08},/*";",27*/
{0x00,0x00,0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x04,0x08,0x10,0x20,0x40,0x00,0x00},/*"<",28*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00},/*"=",29*/
{0x00,0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x04,0x02,0x00,0x00},/*">",30*/
{0x00,0x00,0x00,0x3C,0x42,0x42,0x46,0x20,0x10,0x10,0x10,0x00,0x18,0x18,0x00,0x00},/*"?",31*/
{0x00,0x00,0x00,0x1C,0x22,0x5A,0x55,0x55,0x55,0x55,0x55,0x3A,0x42,0x3C,0x00,0x00},/*"@",32*/
{0x00,0x00,0x00,0x08,0x08,0x18,0x14,0x14,0x24,0x3C,0x22,0x42,0x42,0xE7,0x00,0x00},/*"A",33*/
{0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x1E,0x22,0x42,0x42,0x42,0x22,0x1F,0x00,0x00},/*"B",34*/
{0x00,0x00,0x00,0x7C,0x42,0x42,0x01,0x01,0x01,0x01,0x01,0x42,0x22,0x1C,0x00,0x00},/*"C",35*/
{0x00,0x00,0x00,0x1F,0x22,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1F,0x00,0x00},/*"D",36*/
{0x00,0x00,0x00,0x3F,0x42,0x12,0x12,0x1E,0x12,0x12,0x02,0x42,0x42,0x3F,0x00,0x00},/*"E",37*/
{0x00,0x00,0x00,0x3F,0x42,0x12,0x12,0x1E,0x12,0x12,0x02,0x02,0x02,0x07,0x00,0x00},/*"F",38*/
{0x00,0x00,0x00,0x3C,0x22,0x22,0x01,0x01,0x01,0x71,0x21,0x22,0x22,0x1C,0x00,0x00},/*"G",39*/
{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"H",40*/
{0x00,0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"I",41*/
{0x00,0x00,0x00,0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x11,0x0F},/*"J",42*/
{0x00,0x00,0x00,0x77,0x22,0x12,0x0A,0x0E,0x0A,0x12,0x12,0x22,0x22,0x77,0x00,0x00},/*"K",43*/
{0x00,0x00,0x00,0x07,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x42,0x7F,0x00,0x00},/*"L",44*/
{0x00,0x00,0x00,0x77,0x36,0x36,0x36,0x36,0x36,0x2A,0x2A,0x2A,0x2A,0x6B,0x00,0x00},/*"M",45*/
{0x00,0x00,0x00,0xE3,0x46,0x46,0x4A,0x4A,0x52,0x52,0x52,0x62,0x62,0x47,0x00,0x00},/*"N",46*/
{0x00,0x00,0x00,0x1C,0x22,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x22,0x1C,0x00,0x00},/*"O",47*/
{0x00,0x00,0x00,0x3F,0x42,0x42,0x42,0x42,0x3E,0x02,0x02,0x02,0x02,0x07,0x00,0x00},/*"P",48*/
{0x00,0x00,0x00,0x1C,0x22,0x41,0x41,0x41,0x41,0x41,0x41,0x4D,0x32,0x1C,0x60,0x00},/*"Q",49*/
{0x00,0x00,0x00,0x3F,0x42,0x42,0x42,0x3E,0x12,0x12,0x22,0x22,0x42,0xC7,0x00,0x00},/*"R",50*/
{0x00,0x00,0x00,0x7C,0x42,0x42,0x02,0x04,0x18,0x20,0x40,0x42,0x42,0x3E,0x00,0x00},/*"S",51*/
{0x00,0x00,0x00,0x7F,0x49,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x1C,0x00,0x00},/*"T",52*/
{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},/*"U",53*/
{0x00,0x00,0x00,0xE7,0x42,0x42,0x22,0x24,0x24,0x14,0x14,0x18,0x08,0x08,0x00,0x00},/*"V",54*/
{0x00,0x00,0x00,0x6B,0x2A,0x2A,0x2A,0x2A,0x2A,0x36,0x14,0x14,0x14,0x14,0x00,0x00},/*"W",55*/
{0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x18,0x24,0x24,0x42,0xE7,0x00,0x00},/*"X",56*/
{0x00,0x00,0x00,0x77,0x22,0x22,0x14,0x14,0x08,0x08,0x08,0x08,0x08,0x1C,0x00,0x00},/*"Y",57*/
{0x00,0x00,0x00,0x7E,0x21,0x20,0x10,0x10,0x08,0x04,0x04,0x42,0x42,0x3F,0x00,0x00},/*"Z",58*/
{0x00,0x78,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x78,0x00},/*"[",59*/
{0x00,0x00,0x02,0x04,0x04,0x04,0x08,0x08,0x08,0x10,0x10,0x20,0x20,0x20,0x40,0x40},/*"\",60*/
{0x00,0x1E,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1E,0x00},/*"]",61*/
{0x00,0x18,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"^",62*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF},/*"_",63*/
{0x00,0x06,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"`",64*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x22,0x30,0x2C,0x22,0x32,0x6C,0x00,0x00},/*"a",65*/
{0x00,0x00,0x00,0x00,0x03,0x02,0x02,0x1A,0x26,0x42,0x42,0x42,0x26,0x1A,0x00,0x00},/*"b",66*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x02,0x02,0x02,0x44,0x38,0x00,0x00},/*"c",67*/
{0x00,0x00,0x00,0x00,0x60,0x40,0x40,0x7C,0x42,0x42,0x42,0x42,0x62,0xDC,0x00,0x00},/*"d",68*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x7E,0x02,0x42,0x3C,0x00,0x00},/*"e",69*/
{0x00,0x00,0x00,0x00,0x30,0x48,0x08,0x3E,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"f",70*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x22,0x22,0x1C,0x02,0x3C,0x42,0x42,0x3C},/*"g",71*/
{0x00,0x00,0x00,0x00,0x03,0x02,0x02,0x3A,0x46,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"h",72*/
{0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x0E,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"i",73*/
{0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x38,0x20,0x20,0x20,0x20,0x20,0x20,0x22,0x1E},/*"j",74*/
{0x00,0x00,0x00,0x00,0x03,0x02,0x02,0x72,0x12,0x0A,0x0E,0x12,0x22,0x77,0x00,0x00},/*"k",75*/
{0x00,0x00,0x00,0x08,0x0E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"l",76*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x92,0x92,0x92,0x92,0x92,0xB7,0x00,0x00},/*"m",77*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3B,0x46,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"n",78*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},/*"o",79*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1B,0x26,0x42,0x42,0x42,0x26,0x1A,0x02,0x07},/*"p",80*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x58,0x64,0x42,0x42,0x42,0x64,0x58,0x40,0xE0},/*"q",81*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x77,0x4C,0x04,0x04,0x04,0x04,0x1F,0x00,0x00},/*"r",82*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x42,0x02,0x3C,0x40,0x42,0x3E,0x00,0x00},/*"s",83*/
{0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x3E,0x08,0x08,0x08,0x08,0x48,0x30,0x00,0x00},/*"t",84*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x63,0x42,0x42,0x42,0x42,0x62,0xDC,0x00,0x00},/*"u",85*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x77,0x22,0x22,0x14,0x14,0x08,0x08,0x00,0x00},/*"i",86*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDB,0x91,0x52,0x5A,0x2A,0x24,0x24,0x00,0x00},/*"w",87*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6E,0x24,0x18,0x18,0x18,0x24,0x76,0x00,0x00},/*"x",88*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x08,0x08,0x06},/*"y",89*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x22,0x10,0x08,0x08,0x44,0x7E,0x00,0x00},/*"z",90*/
{0x00,0xC0,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x20,0x20,0x20,0x20,0x20,0xC0,0x00},/*"{",91*/
{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},/*"|",92*/
{0x00,0x03,0x04,0x04,0x04,0x04,0x04,0x04,0x08,0x04,0x04,0x04,0x04,0x04,0x03,0x00},/*"}",93*/
{0x04,0x5A,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"~",94*/
};/* (8 X 16 , 新宋体 )*/


/** ---------- FONT0608 ---------- **/

/*
 * this font is in Totally Vscan Mode.
 * each (uint8_t) represents ONE V line.
 * Total 6 (uint8_t)s stores ALL 6 V lines.
 */
const uint8_t graphic_font0608_defaultType[graphic_asciiCnt][6] =
{
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // sp
	{0x00, 0x00, 0x00, 0x2f, 0x00, 0x00}, // !
	{0x00, 0x00, 0x07, 0x00, 0x07, 0x00}, // "
	{0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14}, // #
	{0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12}, // $
	{0x00, 0x62, 0x64, 0x08, 0x13, 0x23}, // %
	{0x00, 0x36, 0x49, 0x55, 0x22, 0x50}, // &
	{0x00, 0x00, 0x05, 0x03, 0x00, 0x00}, // '
	{0x00, 0x00, 0x1c, 0x22, 0x41, 0x00}, // (
	{0x00, 0x00, 0x41, 0x22, 0x1c, 0x00}, // )
	{0x00, 0x14, 0x08, 0x3E, 0x08, 0x14}, // *
	{0x00, 0x08, 0x08, 0x3E, 0x08, 0x08}, // +
	{0x00, 0x00, 0x00, 0xA0, 0x60, 0x00}, // ,
	{0x00, 0x08, 0x08, 0x08, 0x08, 0x08}, // -
	{0x00, 0x00, 0x60, 0x60, 0x00, 0x00}, // .
	{0x00, 0x20, 0x10, 0x08, 0x04, 0x02}, // /
	{0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
	{0x00, 0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
	{0x00, 0x42, 0x61, 0x51, 0x49, 0x46}, // 2
	{0x00, 0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
	{0x00, 0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
	{0x00, 0x27, 0x45, 0x45, 0x45, 0x39}, // 5
	{0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
	{0x00, 0x01, 0x71, 0x09, 0x05, 0x03}, // 7
	{0x00, 0x36, 0x49, 0x49, 0x49, 0x36}, // 8
	{0x00, 0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
	{0x00, 0x00, 0x36, 0x36, 0x00, 0x00}, // :
	{0x00, 0x00, 0x56, 0x36, 0x00, 0x00}, // ;
	{0x00, 0x08, 0x14, 0x22, 0x41, 0x00}, // <
	{0x00, 0x14, 0x14, 0x14, 0x14, 0x14}, // =
	{0x00, 0x00, 0x41, 0x22, 0x14, 0x08}, // >
	{0x00, 0x02, 0x01, 0x51, 0x09, 0x06}, // ?
	{0x00, 0x32, 0x49, 0x59, 0x51, 0x3E}, // @
	{0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C}, // A
	{0x00, 0x7F, 0x49, 0x49, 0x49, 0x36}, // B
	{0x00, 0x3E, 0x41, 0x41, 0x41, 0x22}, // C
	{0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
	{0x00, 0x7F, 0x49, 0x49, 0x49, 0x41}, // E
	{0x00, 0x7F, 0x09, 0x09, 0x09, 0x01}, // F
	{0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
	{0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
	{0x00, 0x00, 0x41, 0x7F, 0x41, 0x00}, // I
	{0x00, 0x20, 0x40, 0x41, 0x3F, 0x01}, // J
	{0x00, 0x7F, 0x08, 0x14, 0x22, 0x41}, // K
	{0x00, 0x7F, 0x40, 0x40, 0x40, 0x40}, // L
	{0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
	{0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
	{0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
	{0x00, 0x7F, 0x09, 0x09, 0x09, 0x06}, // P
	{0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
	{0x00, 0x7F, 0x09, 0x19, 0x29, 0x46}, // R
	{0x00, 0x46, 0x49, 0x49, 0x49, 0x31}, // S
	{0x00, 0x01, 0x01, 0x7F, 0x01, 0x01}, // T
	{0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
	{0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
	{0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
	{0x00, 0x63, 0x14, 0x08, 0x14, 0x63}, // X
	{0x00, 0x07, 0x08, 0x70, 0x08, 0x07}, // Y
	{0x00, 0x61, 0x51, 0x49, 0x45, 0x43}, // Z
	{0x00, 0x00, 0x7F, 0x41, 0x41, 0x00}, // [
	{0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55}, // 55
	{0x00, 0x00, 0x41, 0x41, 0x7F, 0x00}, // ]
	{0x00, 0x04, 0x02, 0x01, 0x02, 0x04}, // ^
	{0x00, 0x40, 0x40, 0x40, 0x40, 0x40}, // _
	{0x00, 0x00, 0x01, 0x02, 0x04, 0x00}, // '
	{0x00, 0x20, 0x54, 0x54, 0x54, 0x78}, // a
	{0x00, 0x7F, 0x48, 0x44, 0x44, 0x38}, // b
	{0x00, 0x38, 0x44, 0x44, 0x44, 0x20}, // c
	{0x00, 0x38, 0x44, 0x44, 0x48, 0x7F}, // d
	{0x00, 0x38, 0x54, 0x54, 0x54, 0x18}, // e
	{0x00, 0x08, 0x7E, 0x09, 0x01, 0x02}, // f
	{0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C}, // g
	{0x00, 0x7F, 0x08, 0x04, 0x04, 0x78}, // h
	{0x00, 0x00, 0x44, 0x7D, 0x40, 0x00}, // i
	{0x00, 0x40, 0x80, 0x84, 0x7D, 0x00}, // j
	{0x00, 0x7F, 0x10, 0x28, 0x44, 0x00}, // k
	{0x00, 0x00, 0x41, 0x7F, 0x40, 0x00}, // l
	{0x00, 0x7C, 0x04, 0x18, 0x04, 0x78}, // m
	{0x00, 0x7C, 0x08, 0x04, 0x04, 0x78}, // n
	{0x00, 0x38, 0x44, 0x44, 0x44, 0x38}, // o
	{0x00, 0xFC, 0x24, 0x24, 0x24, 0x18}, // p
	{0x00, 0x18, 0x24, 0x24, 0x18, 0xFC}, // q
	{0x00, 0x7C, 0x08, 0x04, 0x04, 0x08}, // r
	{0x00, 0x48, 0x54, 0x54, 0x54, 0x20}, // s
	{0x00, 0x04, 0x3F, 0x44, 0x40, 0x20}, // t
	{0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
	{0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
	{0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
	{0x00, 0x44, 0x28, 0x10, 0x28, 0x44}, // x
	{0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C}, // y
	{0x00, 0x44, 0x64, 0x54, 0x4C, 0x44}, // z
	{0x14, 0x14, 0x14, 0x14, 0x14, 0x14}, // { , incorrect
	{0x14, 0x14, 0x14, 0x14, 0x14, 0x14}, // | , incorrect
	{0x14, 0x14, 0x14, 0x14, 0x14, 0x14}, // } , incorrect
	{0x14, 0x14, 0x14, 0x14, 0x14, 0x14}, // ~
};

/* @} */
