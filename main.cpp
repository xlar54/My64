#include "main.h"
#include <iostream>
#include <fstream>
#include "MOS6510.h"
#include "MOS6569.h"


void loadroms(void);
void loadcputest(void);

MOS6510 cpu;
MOS6569 vic2;

int main(int argc, char* argv[])
{
    loadroms();
    //loadcputest();
    //cpu.memory.basicIn = false;
    //cpu.memory.ioIn = false;
   // cpu.memory.kernalIn = false;
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
}

void loadcputest(void) {

    UINT8_T buffer[65535];
    std::ifstream tests("6502_functional_test.bin", std::ios::in | std::ios::binary);
    tests.read((char*)buffer, 8192);

    if (!tests) {
        std::cout << "Couldnt load test file!\n";
        exit(0);
    }

    for (int x = 0; x < 65535; x++)
        cpu.memory.LoadRAM(buffer);


}