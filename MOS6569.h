#pragma once
#ifdef _WIN32
#include "./win32/Screen.h"
#endif

#include "types.h"

class Memory;

class MOS6569
{
private:
	Screen screen;
	UINT16_T bankAddress;
	UINT16_T screenAddress;
	UINT16_T charRomAddress;
	
	UINT8_T reg[0x2e];
	UINT8_T color_ram[COLOR_RAM_END-COLOR_RAM_START];

public:
	MOS6569(Memory* memory);
	void Bank(UINT8_T val);
	UINT8_T Read(UINT16_T address);
	void Write(UINT16_T address, UINT8_T val);
	void ColorWrite(UINT16_T address, UINT8_T color);
	UINT8_T ColorRead(UINT16_T address);
	void Text(UINT16_T pos, UINT8_T value);
	void Refresh(void);
	void SetPixel(UINT16_T x, UINT8_T y, UINT8_T coloridx);

};

