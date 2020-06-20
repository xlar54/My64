#pragma once
#include "Machine.h"
#include "types.h"
#include "MOS6569.h"

class MOS6569;

class Memory
{
private:
	UINT8_T ram[65535] = {};
	UINT8_T basic[8192] = {};
	UINT8_T kernal[8192] = {};
	UINT8_T charset[4096] = {};
	UINT16_T watchVicFrom;
	UINT16_T watchVicTo;
	MOS6569* vic2;

	bool loram = true;
	bool hiram = true;
	bool charen = true;
	bool hiANDlo;
	bool hiORlo;

public:
	Memory();
	
	UINT8_T Peek(UINT16_T address);
	UINT16_T PeekW(UINT16_T address);
	void Poke(UINT16_T address, UINT8_T value);
	UINT8_T VicPeek(UINT16_T address);
	void LoadBasic(UINT8_T* data);
	void LoadKernal(UINT8_T* data);
	void LoadCharset(UINT8_T* data);
	void MapIO(MOS6569* vic2);
	
	void SetVicWatchRange(UINT16_T startingAddress, UINT16_T endAddress);
	void ioport(UINT8_T port);
	UINT8_T ioIn(UINT16_T address);
	void ioOut(UINT16_T address, UINT8_T value);
	
	void Text(UINT16_T pos, UINT8_T value);
	MOS6569* GetVIC(void);

};

