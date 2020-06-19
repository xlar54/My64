#pragma once
#ifdef _WIN32
#include "./win32/Screen.h"
#endif

#include "types.h"

class MOS6569
{
private:
	Screen screen;

public:
	MOS6569(void);
	void Text(UINT16_T pos, UINT8_T value);
	void SetPixel(UINT16_T x, UINT8_T y, UINT8_T coloridx);

};

