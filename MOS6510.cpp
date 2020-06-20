#include "MOS6510.h"
#include <iostream>

MOS6510::MOS6510(void) {
	a = 0;
	x = 0;
	y = 0;
	p = 0;
	s = 0;
	pc = 0;
	setflag(z, true);
	setflag(i, true);
}

void MOS6510::dump(UINT8_T instr) {

	std::cout << std::hex << "PC:" << (int)pc << " OP:" << (int)instr << "  A:" << (int)a << " X:" << (int)x << " Y:" << (int)y << " SP:" << (int)s << " SR:" << (int)p << "\n";
}

void MOS6510::run(UINT16_T address) {

	bool running = true;
	
	pc = address;
	startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

	UINT8_T opcode = 0;  

	while (running)
	{
		opcode = memory.Peek(pc);

		//if (pc == 0xfd9a)
		//	trace = true;

		inc(1);

		switch (opcode)
		{
			case 0x69: { adc(imm()); break; }
			case 0x65: { adc(zp()); break; }
			case 0x75: { adc(zpx()); break; }
			case 0x6D: { adc(abs()); break; }
			case 0x7D: { adc(absx()); break; }
			case 0x79: { adc(absy()); break; }
			case 0x61: { adc(indx()); break; }
			case 0x71: { adc(indy()); break; }

			case 0x29: { and_(imm()); break; }
			case 0x25: { and_(zp()); break; }
			case 0x35: { and_(zpx()); break; }
			case 0x2D: { and_(abs()); break; }
			case 0x3D: { and_(absx()); break; }
			case 0x39: { and_(absy()); break; }
			case 0x21: { and_(indx()); break; }
			case 0x31: { and_(indy()); break; }

			case 0x0A: { asl_a(); break; }
			case 0x06: { asl(zp()); break; }
			case 0x16: { asl(zpx()); break; }
			case 0x0E: { asl(abs()); break; }
			case 0x1E: { asl(absx()); break; }

			case 0x90: { bfc(c); break; } //bcc 
			case 0xB0: { bfs(c); break; } //bcs
			case 0xF0: { bfs(z); break; } //beq

			case 0x24: { bit(zp()); break; }
			case 0x2C: { bit(abs()); break; }

			case 0x30: { bfs(n); break; } //bmi
			case 0xD0: { bfc(z); break; } //bne
			case 0x10: { bfc(n); break; } //bpl
			case 0x00: { brk();  break; }
			case 0x50: { bfc(v); break; } //bvc
			case 0x70: { bfs(v); break; } //bvs

			case 0x18: { setflag(c, false);	break; } //clc
			case 0xD8: { setflag(d, false); break; } //cld
			case 0x58: { setflag(i, false); break; } //cli
			case 0xB8: { setflag(v, false);	break; }//clv

			case 0xC9: { cmp(imm()); break; }
			case 0xC5: { cmp(zp()); break; }
			case 0xD5: { cmp(zpx()); break; }
			case 0xCD: { cmp(abs()); break; }
			case 0xDD: { cmp(absx()); break; }
			case 0xD9: { cmp(absy()); break; }
			case 0xC1: { cmp(indx()); break; }
			case 0xD1: { cmp(indy()); break; }

			case 0xE0: { cpx(imm()); break; }
			case 0xE4: { cpx(zp()); break; }
			case 0xEC: { cpx(abs()); break; }

			case 0xC0: { cpy(imm()); break; }
			case 0xC4: { cpy(zp()); break; }
			case 0xCC: { cpy(abs()); break; }

			case 0xC6: { dec_(zp()); break; }
			case 0xD6: { dec_(zpx()); break; }
			case 0xCE: { dec_(abs()); break; }
			case 0xDE: { dec_(absx()); break; }

			case 0xCA: { dex(); break; }
			case 0x88: { dey(); break; }

			case 0x49: { eor(imm()); break; }
			case 0x45: { eor(zp()); break; }
			case 0x55: { eor(zpx()); break; }
			case 0x4D: { eor(abs()); break; }
			case 0x5D: { eor(absx()); break; }
			case 0x59: { eor(absy()); break; }
			case 0x41: { eor(indx()); break; }
			case 0x51: { eor(indy()); break; }

			case 0xE6: { inc_(zp()); break; }
			case 0xF6: { inc_(zpx()); break; }
			case 0xEE: { inc_(abs()); break; }
			case 0xFE: { inc_(absx()); break; }

			case 0xE8: { inx(); break; }
			case 0xC8: { iny(); break; }

			case 0x4C: { jmp(abs()); break; }
			case 0x6C: { jmp(ind()); break; }

			case 0x20: { jsr(abs()); break; }

			case 0xA9: { lda(imm()); break; }
			case 0xA5: { lda(zp()); break; }
			case 0xB5: { lda(zpx()); break; }
			case 0xAD: { lda(abs()); break; }
			case 0xBD: { lda(absx()); break; }
			case 0xB9: { lda(absy()); break; }
			case 0xA1: { lda(indx()); break; }
			case 0xB1: { lda(indy()); break; }

			case 0xA2: { ldx(imm()); break; }
			case 0xA6: { ldx(zp()); break; }
			case 0xB6: { ldx(zpy()); break; }
			case 0xAE: { ldx(abs()); break; }
			case 0xBE: { ldx(absy()); break; }

			case 0xA0: { ldy(imm()); break; }
			case 0xA4: { ldy(zp()); break; }
			case 0xB4: { ldy(zpx()); break; }
			case 0xAC: { ldy(abs()); break; }
			case 0xBC: { ldy(absx()); break; }

			case 0x4A: { lsr_a(); break; }
			case 0x46: { lsr(zp()); break; }
			case 0x56: { lsr(zpx()); break; }
			case 0x4E: { lsr(abs()); break; }
			case 0x5E: { lsr(absx()); break; }

			case 0xEA: { break; } // nop

			case 0x09: { ora(imm()); break; }
			case 0x05: { ora(zp()); break; }
			case 0x15: { ora(zpx()); break; }
			case 0x0D: { ora(abs()); break; }
			case 0x1D: { ora(absx()); break; }
			case 0x19: { ora(absy()); break; }
			case 0x01: { ora(indx()); break; }
			case 0x11: { ora(indy()); break; }

			case 0x48: { push(a);		break; } //pha
			case 0x08: { push(p);		break; } //php
			case 0x68: { pla();			break; }
			case 0x28: { p = pull();	break; } //plp

			case 0x2A: { rol_a(); break; }
			case 0x26: { rol(zp()); break; }
			case 0x36: { rol(zpx()); break; }
			case 0x2E: { rol(abs()); break; }
			case 0x3E: { rol(absx()); break; }

			case 0x6A: { ror_a(); break; }
			case 0x66: { ror(zp()); break; }
			case 0x76: { ror(zpx()); break; }
			case 0x6E: { ror(abs()); break; }
			case 0x7E: { ror(absx()); break; }

			case 0x40: { rti(); break; }
			case 0x60: { rts(); break; }

			case 0xE9: { sbc(imm()); break; }
			case 0xE5: { sbc(zp()); break; }
			case 0xF5: { sbc(zpx()); break; }
			case 0xED: { sbc(abs()); break; }
			case 0xFD: { sbc(absx()); break; }
			case 0xF9: { sbc(absy()); break; }
			case 0xE1: { sbc(indx()); break; }
			case 0xF1: { sbc(indy()); break; }

			case 0x38: { setflag(c, true); break; }   //sec
			case 0xF8: { setflag(d, true); break; }  //sed
			case 0x78: { setflag(i, true); break; }  //sei

			case 0x85: { sta(zp()); break; }
			case 0x95: { sta(zpx()); break; }
			case 0x8D: { sta(abs()); break; }
			case 0x9D: { sta(absx()); break; }
			case 0x99: { sta(absy()); break; }
			case 0x81: { sta(indx()); break; }
			case 0x91: { sta(indy()); break; }

			case 0x86: { stx(zp()); break; }
			case 0x96: { stx(zpy()); break; }
			case 0x8E: { stx(abs()); break; }

			case 0x84: { sty(zp()); break; }
			case 0x94: { sty(zpx()); break; }
			case 0x8C: { sty(abs()); break; }

			case 0xAA: { tax(); break; }
			case 0xA8: { tay(); break; }
			case 0xBA: { tsx(); break; }
			case 0x8A: { txa(); break; }
			case 0x9A: { txs(); break; }
			case 0x98: { tya(); break; }
			default:
			{ 
				running = false;
				std::cout << "\n\nFATAL ERROR: Unknown opcode:" << (int)opcode << " - PC=" << (int)pc << "\n\n";
				break; };
		}

		currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

		if ((currentTime.count() - startTime.count()) * 6 > timer50hz * 5)
		{
			irq();
			//std::cout << (currentTime.count() - startTime.count()) * 6 << "   " << timer50hz*5 << "\n";
		}

		if (trace)
			dump(opcode);

	}



}

