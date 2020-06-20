#include "Memory.h"
#include "MOS6569.h"
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

	// At power up, this value is set to 239
	ram[0x00] = 0xff;
	ram[0x01] = 0xff;
	
}

void Memory::MapIO(MOS6569* vic2)
{
	this->vic2 = vic2;
}

MOS6569* Memory::GetVIC(void)
{
	return vic2;
}

void Memory::SetVicWatchRange(UINT16_T startingAddress, UINT16_T endAddress)
{
	watchVicFrom = startingAddress;
	watchVicTo = endAddress;
}

void Memory::ioport(UINT8_T port)
{
	UINT8_T maskedPort;

	maskedPort = port & ram[0x0000];
	loram = maskedPort & 1;
	hiram = maskedPort & 2;
	charen = maskedPort & 4;

	hiANDlo = hiram && loram;
	hiORlo = hiram || loram;
}

UINT8_T Memory::ioIn(UINT16_T address)
{
	if (address >= IO_START and address <= IO_START + 0x2e)
		return vic2->Read(address);

	if (address >= COLOR_RAM_START and address <= COLOR_RAM_END)
		return vic2->ColorRead(address);

	return 0xff;
}

void Memory::ioOut(UINT16_T address, UINT8_T value)
{
	if (address >= IO_START and address <= IO_START + 0x2e)
		vic2->Write(address, value);

	if (address >= COLOR_RAM_START and address <= COLOR_RAM_END)
		vic2->ColorWrite(address, value);

	if (address == 0xdd00)
		vic2->Bank(value);
}

void Memory::Poke(UINT16_T address, UINT8_T value)
{
	ram[address] = value;

	if ((address >= watchVicFrom) && (address <= watchVicTo))
		vic2->Text(address - watchVicFrom, value);

	if (address == 0x0001)
		ioport(value);

	if (address >= SCREEN_START && address <= SCREEN_END)
		vic2->Text(address - SCREEN_START, value);

	if (address >= IO_START && address <= IO_END)
		if (hiORlo)
			ioOut(address, value);

}

UINT8_T Memory::Peek(UINT16_T address)
{
	if (address >= BASIC_START && address <= BASIC_END)
	{
		if (hiANDlo)
			return basic[address - BASIC_START];
		else
			return ram[address];
	}
		
	if (address >= IO_START && address <= IO_END)
	{
		if (hiORlo)
		{
			if (charen)
				return ioIn(address);
			else
				return charset[address - IO_START];
		}
		else
			return ram[address];
	}	

	if (address >= KERNAL_START && address <= KERNAL_END)
	{
		if (hiram)
			return kernal[address - KERNAL_START];
		else
			return ram[address];
	}

	return ram[address];
}

UINT16_T Memory::PeekW(UINT16_T address)
{
	UINT8_T lo = Peek(address);
	UINT8_T hi = Peek(address + 1);

	return hi * 256 + lo;
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
		return charset[(address + 0xc000) - 0xd000];

	if (address >= 0x9000 && address <= 0x9fff)
		return charset[(address + 0x4000) - 0xd000];

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