#pragma once
#include "types.h"
#include "MOS6569.h"

class Memory
{
private:
	UINT8_T ram[65535];
	UINT8_T basic[8192];
	UINT8_T kernal[8192];
	UINT8_T charset[4096];
	MOS6569* vic2;

public:
	Memory();
	
	bool ioIn = true;
	bool basicIn = true;
	bool kernalIn = true;
	
	UINT8_T Peek(UINT16_T address);
	UINT16_T PeekW(UINT16_T address);
	void Poke(UINT16_T address, UINT8_T value);
	UINT8_T VicPeek(UINT16_T address);
	void LoadBasic(UINT8_T* data);
	void LoadKernal(UINT8_T* data);
	void LoadCharset(UINT8_T* data);
	void MapIO(MOS6569* vic2);
	void Text(UINT16_T pos, UINT8_T value);

};

