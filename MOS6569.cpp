#include "MOS6569.h"

MOS6569::MOS6569(void)
{

}

void MOS6569::Text(UINT16_T pos, UINT8_T value)
{
	screen.Text(pos, value);
}

void MOS6569::SetPixel(UINT16_T x, UINT8_T y, UINT8_T coloridx)
{
	screen.SetPixel(x, y, coloridx);
}

