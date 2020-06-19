#include "main.h"
#include <iostream>
#include <fstream>
#include "MOS6510.h"
#include "MOS6569.h"

void loadroms(void);

MOS6510 cpu;
MOS6569 vic2;

int main(int argc, char* argv[])
{
    loadroms();

    cpu.memory.MapIO(&vic2);
	cpu.run(cpu.memory.PeekW(0xfffc));

    return 0;
	
}

void loadroms(void) {

    UINT8_T buffer[8192];
    std::ifstream basicRom("basic", std::ios::in | std::ios::binary);
    basicRom.read((char*)buffer, 8192);
    
    if (!basicRom) {
        std::cout << "Couldnt load basic rom file!\n";
        exit(0);
    }

    cpu.memory.LoadBasic(buffer);

    std::ifstream kernelRom("kernal", std::ios::in | std::ios::binary);
    kernelRom.read((char*)buffer, 8192);

    if (!kernelRom) {
        std::cout << "Couldnt load kernal rom file!\n";
        exit(0);
    }

    cpu.memory.LoadKernal(buffer);

    std::ifstream charsetRom("chargen", std::ios::in | std::ios::binary);
    charsetRom.read((char*)buffer, 4096);

    if (!charsetRom) {
        std::cout << "Couldnt load character rom file!\n";
        exit(0);
    }

    cpu.memory.LoadCharset(buffer);
}

