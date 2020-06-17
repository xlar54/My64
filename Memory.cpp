#include "Memory.h"
#include <iostream>


Memory::Memory() {
	
	for (int t = 0; t < 65535; t++)
		ram[t] = 0;

	for (int t = 0; t < 8192; t++)
		basic[t] = 0;

	for (int t = 0; t < 8192; t++)
		kernal[t] = 0;

	
}

void Memory::MapIO(MOS6569* vic2)
{
	this->vic2 = vic2;
}

UINT8_T Memory::Peek(UINT16_T address)
{
	if (basicIn && (address >= 0xa000 && address <= 0xbfff))
		return basic[address - 0xa000];
		
	if (ioIn && (address >= 0xd000 && address <= 0xdfff))
	{
		if (address == 0xd012)
			return 0x00;
		else
			return 0xff;
	}	

	if (kernalIn && (address >= 0xe000 && address <= 0xffff))
	{
		return kernal[address - 0xe000];
	}

	return ram[address];
}

UINT16_T Memory::PeekW(UINT16_T address)
{
	if (basicIn && (address >= 0xa000 && address <= 0xbfff))
		return basic[address - 0xa000 + 1] * 256 + basic[address - 0xa000];

	if (ioIn && (address >= 0xd000 && address <= 0xdfff))
		return 0xffff;

	if (kernalIn && (address >= 0xe000 && address <= 0xffff))
		return kernal[address - 0xe000 + 1] * 256 + kernal[address - 0xe000];

	return ram[address + 1] * 256 + ram[address];
}

void Memory::Poke(UINT16_T address, UINT8_T value)
{
	ram[address] = value;

	if (address >= 0x0400 && address <= 0x07e7)
		vic2->Text(address-0x400, value);
		
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

void Memory::LoadRAM(UINT8_T* data)
{
	for (int t = 0; t < 8192; t++)
		ram[t] = data[t];
}