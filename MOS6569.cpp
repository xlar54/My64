#include "Machine.h"
#include "Memory.h"
#include "MOS6569.h"

Memory *mem;

MOS6569::MOS6569(Memory* memory) :
	bankAddress(0),
	charRomAddress(0),
	screenAddress(0)
{
	mem = memory;

	for (int x = 0; x < (COLOR_RAM_END - COLOR_RAM_START); color_ram[x++] = 0);
	for (int x = 0; x < 0x2e; reg[x++] = 0);
}

void MOS6569::Bank(UINT8_T val)
{
	bankAddress = (val & 3) * 0x4000;
	mem->SetVicWatchRange(bankAddress + screenAddress, bankAddress + screenAddress + 999);
	
}

UINT8_T MOS6569::Read(UINT16_T address)
{
	switch (address)
	{
	case 0xd012:
		return 0x00;
	default:
		return reg[address - IO_START];

	}
}

void MOS6569::Write(UINT16_T address, UINT8_T val)
{
	reg[address - IO_START] = val;

	switch (address)
	{
		case 0xd018:
		{
			screenAddress = (val >> 4) * SCREEN_START;
			mem->SetVicWatchRange(bankAddress + screenAddress, bankAddress + screenAddress + 999);
			charRomAddress = (val & 0x0e) * SCREEN_START;
		}
	}
}

void MOS6569::ColorWrite(UINT16_T address, UINT8_T color)
{
	color_ram[address - COLOR_RAM_START] = color;
}

UINT8_T MOS6569::ColorRead(UINT16_T address)
{
	return color_ram[address - COLOR_RAM_START];
}

void MOS6569::Text(UINT16_T pos, UINT8_T val)
{
	screen.Console(pos, val);

	int x0, y0, x, y;

	x0 = (pos % MAX_COLS) * 8;
	y0 = (pos / MAX_COLS) * 8;

	for (x = 0; x<= 7; x++)
		for (y = 0; y <= 7; y++)
		{
			//bankAddress +
			UINT16_T b = charRomAddress + val * 8 + y;
			UINT8_T m = mem->VicPeek(b);

			if (m & (0x80 >> x))
				screen.SetPixel(x0 + x, y0 + y, color_ram[pos]);
			else
				screen.SetPixel(x0 + x, y0 + y, reg[0x21]);
		}
}

void MOS6569::Refresh(void)
{
	screen.Refresh();
}

void MOS6569::SetPixel(UINT16_T x, UINT8_T y, UINT8_T coloridx)
{
	screen.SetPixel(x, y, coloridx);
}

