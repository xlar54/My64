#include "main.h"
#include <iostream>
#include <fstream>

#ifdef __VIC20__
    const char* basicrom = "./roms/vic20/basic";
    const char* kernalrom = "./roms/vic20/kernal";
    const char* charrom = "./roms/vic20/chargen";
#endif

#ifdef __C64__
    const char* basicrom = "./roms/c64/basic";
    const char* kernalrom = "./roms/c64/kernal";
    const char* charrom = "./roms/c64/chargen";
#endif

MOS6510 cpu;
MOS6569 vic2(&cpu.memory);

int main(int argc, char* argv[])
{
    loadroms();

    cpu.memory.MapIO(&vic2);
	cpu.run(cpu.memory.PeekW(0xfffc));

    return 0;
	
}

void loadroms(void) {

    UINT8_T buffer[8192];
    std::ifstream basicRom(basicrom, std::ios::in | std::ios::binary);
    basicRom.read((char*)buffer, 8192);
    
    if (!basicRom) {
        std::cout << "Couldnt load basic rom file!\n";
        exit(0);
    }

    cpu.memory.LoadBasic(buffer);

    std::ifstream kernelRom(kernalrom, std::ios::in | std::ios::binary);
    kernelRom.read((char*)buffer, 8192);

    if (!kernelRom) {
        std::cout << "Couldnt load kernal rom file!\n";
        exit(0);
    }

    cpu.memory.LoadKernal(buffer);

    std::ifstream charsetRom(charrom, std::ios::in | std::ios::binary);
    charsetRom.read((char*)buffer, 4096);

    if (!charsetRom) {
        std::cout << "Couldnt load character rom file!\n";
        exit(0);
    }

    cpu.memory.LoadCharset(buffer);
}

