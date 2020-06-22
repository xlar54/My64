#ifndef __MACHINE_H__
#define __MACHINE_H__

#include "types.h"

const UINT16_T BASIC_START = 0xa000;
const UINT16_T BASIC_END = 0xbfff;
const UINT16_T KERNAL_START = 0xe000;
const UINT16_T KERNAL_END = 0xffff;
const UINT16_T IO_START = 0xd000;
const UINT16_T IO_END = 0xdfff;
const UINT16_T CHARSET_START = 0xd000;
const UINT16_T CHARSET_END = 0xdfff;
const UINT16_T SCREEN_START = 0x0400;
const UINT16_T SCREEN_END = 0x07e7;
const UINT16_T KB_BUFF_START = 0x277;
const UINT16_T KB_BUFF_END = 0x280;
const UINT8_T ZP_CHARS_IN_KBUFFER = 0xc6;
const UINT8_T MAX_COLS = 40;
const UINT8_T MAX_ROWS = 23;
const UINT16_T START_OF_BASIC = 0x0800;
const UINT16_T COLOR_RAM_START = 0xd800;
const UINT16_T COLOR_RAM_END = 0xdbff;

#endif
