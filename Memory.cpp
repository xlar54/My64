#include "Memory.h"
#include <iostream>

Memory::Memory() {
	
	for (int t = 0; t < 65535; t++)
		ram[t] = 0;

	for (int t = 0; t < 8192; t++)
		basic[t] = 0;

	for (int t = 0; t < 8192; t++)
		kernal[t] = 0;

	for (int t = 0; t < 4096; t++)
		charset[t] = 0;
	
}

void Memory::MapIO(MOS6569* vic2)
{
	this->vic2 = vic2;
}

UINT8_T Memory::Peek(UINT16_T address)
{
	if (basicIn && (address >= BASIC_START && address <= BASIC_END))
		return basic[address - BASIC_START];
		
	if (ioIn && (address >= 0xd000 && address <= 0xdfff))
	{
		if (address == 0xd012)
			return 0x00;
		else
			return 0xff;
	}	

	if (kernalIn && (address >= KERNAL_START && address <= KERNAL_END))
	{
		return kernal[address - KERNAL_START];
	}

	return ram[address];
}

UINT16_T Memory::PeekW(UINT16_T address)
{
	if (basicIn && (address >= BASIC_START && address <= BASIC_END))
		return basic[address - BASIC_START + 1] * 256 + basic[address - BASIC_START];

	if (ioIn && (address >= 0xd000 && address <= 0xdfff))
		return 0xffff;

	if (kernalIn && (address >= KERNAL_START && address <= KERNAL_END))
		return kernal[address - KERNAL_START + 1] * 256 + kernal[address - KERNAL_START];

	return ram[address + 1] * 256 + ram[address];
}

void Memory::Poke(UINT16_T address, UINT8_T value)
{
	ram[address] = value;

	if (address >= SCREEN_START && address <= SCREEN_END)
		vic2->Text(address-SCREEN_START, value);
		
}

void Memory::Text(UINT16_T pos, UINT8_T value)
{
	int x0 = (pos % MAX_COLS) * 8;
	int y0 = (pos / MAX_COLS) * 8;
	for (int x = 0; x < 7; x++)
		for (int y = 0; y < 7; y++)
			if (VicPeek(0x1000 + value * 8 + y) & (0x80 >> x))
				vic2->SetPixel(x0 + x, y0 + y, 14); //light blue
			else
				vic2->SetPixel(x0 + x, y0 + y, 6); //blue
}


UINT8_T Memory::VicPeek(UINT16_T address)
{
	if (address >= 0x1000 && address <= 0x1fff)
		return charset[address + 0xc000];

	if (address >= 0x9000 && address <= 0x9fff)
		return charset[address + 0x4000];

	return ram[address];
}

void Memory::LoadBasic(UINT8_T* data)
{
	for (int t = 0; t < 8192; t++)
		basic[t] = data[t];
}

void Memory::LoadKernal(UINT8_T* data)
{
	for (int t = 0; t < 8192; t++)
		kernal[t] = data[t];
}

void Memory::LoadCharset(UINT8_T* data)
{
	for (int t = 0; t < 4096; t++)
		charset[t] = data[t];
}