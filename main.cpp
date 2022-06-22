// Tell SDL not to mess with main()
#define SDL_MAIN_HANDLED

#include <cstdio>
#include <iostream>
#include <fstream>
#include "Machine.h"
#include "MOS6510.h"
#include "MOS6569.h"

using namespace std;

const char* basicrom = "./roms/basic";
const char* kernalrom = "./roms/kernal";
const char* charrom = "./roms/chargen";

MOS6510 cpu;
MOS6569 vic2(&cpu.memory);

void loadroms(void); 

int main(int argc, char* argv[])
{
    loadroms();

    cpu.memory.MapIO(&vic2);
	cpu.run(cpu.memory.PeekW(0xfffc));

    return 0;
	
}

void loadroms(void) {

    UINT8_T buffer[8192];
    ifstream basicRom(basicrom, ios::in | ios::binary);
    basicRom.read((char*)buffer, 8192);
    
    if (!basicRom) {
        cout << "Couldnt load basic rom file!\n";
        exit(0);
    }

    cpu.memory.LoadBasic(buffer);

    ifstream kernelRom(kernalrom, ios::in | ios::binary);
    kernelRom.read((char*)buffer, 8192);

    if (!kernelRom) {
        cout << "Couldnt load kernal rom file!\n";
        exit(0);
    }

    cpu.memory.LoadKernal(buffer);

    ifstream charsetRom(charrom, ios::in | ios::binary);
    charsetRom.read((char*)buffer, 4096);

    if (!charsetRom) {
        cout << "Couldnt load character rom file!\n";
        exit(0);
    }

    cpu.memory.LoadCharset(buffer);
}

