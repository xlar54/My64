#pragma once
#include <chrono>
#include "Memory.h"
#include "types.h"
#include <conio.h> //not portable
#include <iostream>
#include "win32\Keyboard.h"

class MOS6510
{
private:

	bool trace = false;

	UINT8_T a = 0;
	UINT8_T x = 0;
	UINT8_T y = 0;
	UINT8_T p = 0;
	UINT8_T s = 0xff;
	UINT16_T pc = 0;

	std::chrono::milliseconds startTime;
	std::chrono::milliseconds currentTime;
	Keyboard keyboard;

	long long  timer50hz = 20;

	void inc(UINT8_T step) { pc += step; };
	void dec(UINT8_T step) { pc -= step; };
	UINT8_T hi(UINT16_T val) { return ((val >> 8) & 0xff); };
	UINT8_T lo(UINT16_T val) { return (val & 0xff); };

	UINT16_T	imm()	{ UINT16_T v = pc; inc(1); return v; };
	UINT8_T		zp()	{ UINT8_T v = memory.Peek(pc); inc(1); return v; };
	UINT8_T		zpx()	{ UINT8_T v = memory.Peek(pc)+x; inc(1); return v; };
	UINT8_T		zpy()	{ UINT8_T v = memory.Peek(pc)+y; inc(1); return v; };
	UINT16_T	abs()	{ UINT16_T v = memory.PeekW(pc); inc(2); return v; };
	UINT16_T	absx()	{ UINT16_T v = memory.PeekW(pc) + x; inc(2); return v; };
	UINT16_T	absy()	{ UINT16_T v = memory.PeekW(pc) + y; inc(2); return v; };
	UINT16_T	ind()	{ UINT16_T v = memory.PeekW(memory.PeekW(pc)); inc(2); return v; };
	UINT16_T	indx()	{ UINT16_T v = memory.PeekW(memory.Peek(pc)+x); inc(1); return v; };
	UINT16_T	indy()	{ UINT16_T v = memory.PeekW(memory.Peek(pc))+y; inc(1); return v; };

	//********************************************************************
	// Actions on Status Register(P)    NV.BDIZC *
	//********************************************************************)
	static const UINT8_T c = 0x01;
	static const UINT8_T z = 0x02;
	static const UINT8_T i = 0x04;
	static const UINT8_T d = 0x08;
	static const UINT8_T b = 0x10;
	static const UINT8_T v = 0x40;
	static const UINT8_T n = 0x80;

	//**********************************************************************
	// Addkey
	//**********************************************************************
	void addkey()
	{
		UINT8_T ch = keyboard.Getkey();

		if (ch != 0)
		{
			UINT8_T buff = memory.Peek(ZP_CHARS_IN_KBUFFER);

			if (buff < 10)
			{
				memory.Poke(KB_BUFF_START + buff, ch);
				memory.Poke(ZP_CHARS_IN_KBUFFER, buff + 1);
			}
		}


	}

	void setflag(UINT8_T flag, bool status)
	{
		if (status) {
			p = p | flag;
		}
		else {
			p = p & ~flag;
		}
	}
	
	bool flagset(UINT8_T flag)
	{
		return (p & flag);
	}

	//********************************************************************
	// Stack operations *
	//********************************************************************)
	void push(UINT8_T byte)
	{
		memory.Poke(0x100 + s, byte);
		s--;
	};

	UINT8_T pull(void)
	{
		s++;
		return memory.Peek(0x100 + s);
	};

	//******************************************************************
	// generic branching instructions *
	//******************************************************************)
	void bfs(UINT8_T flag)
	{
		if (flagset(flag)) {
			pc = pc + ((signed char)memory.Peek(pc)) + 1;
		}
		else inc(1);
	};
	
	void bfc(UINT8_T flag)
	{
		if (flagset(flag)) {
			inc(1);
		}
		else {
			pc = pc + ((signed char)memory.Peek(pc)) + 1;
		};
	};

	//********************************************************************
	// Instructions *
	//********************************************************************)
	void adc(UINT16_T address)
	{
		UINT16_T h;
		h = a + memory.Peek(address);
		if (flagset(c)) { h++; };
		a = h;
		setflag(c, h > 0xff);
		setflag(z, a == 0);
		setflag(n, a >= 0x80);
		h = h + 0x80;
		setflag(v, (h > 0xff) | (h < 0));
	};

	void sbc(UINT16_T address)
	{
		UINT16_T h;
		h = a - memory.Peek(address);
		if (!flagset(c)) { h--; };
		a = h;
		setflag(c, h <= 0xff);
		setflag(z, a == 0);
		setflag(n, a >= 0x80);
		h = h + 0x80;
		setflag(v, (h > 0xff) | (h < 0));
	};

	void and_ (UINT16_T address)
	{
		a = a & memory.Peek(address);
		setflag(z, a == 0);
		setflag(n, a >= 0x80);
	};

	void ora(UINT16_T address)
	{
		a = a | memory.Peek(address);
		setflag(z, a == 0);
		setflag(n, a >= 0x80);
	};

	void asl_a(void)
	{
		setflag(c, (a & 0x80));
		a = a << 1;
		setflag(z, a == 0);
		setflag(n, a >= 0x80);
	};

	void asl(UINT16_T address)
	{
		UINT8_T b;
		b = memory.Peek(address);
		setflag(c, (b & 0x80));
		b = b << 1;
		memory.Poke(address, b);
		setflag(z, b == 0);
		setflag(n, b >= 0x80);
	};

	void rol_a(void)
	{
		bool bit;
		bit = flagset(c);
		setflag(c, (a & 0x80));
		a = a << 1;
		if (bit) { a = a | 0x01; };
		setflag(z, a == 0);
		setflag(n, a >= 0x80);
	};

	void rol(UINT16_T address)
	{
		UINT8_T b;
		bool bit;

		b = memory.Peek(address);
		bit = flagset(c);
		setflag(c, (b & 0x80));
		b = b << 1;
		if (bit) { b = b | 0x01; };
		memory.Poke(address, b);
		setflag(z, b == 0);
		setflag(n, b >= 0x80);
	};

	void ror_a(void)
	{
		bool bit;
		bit = flagset(c);
		setflag(c, (a & 0x01));
		a = a >> 1;
		if (bit) { a = a | 0x80; };
		setflag(z, a == 0);
		setflag(n, a >= 0x80);
	};

	void ror(UINT16_T address)
	{
		UINT8_T b;
		bool bit;
		b = memory.Peek(address);
		bit = flagset(c);
		setflag(c, (b & 0x01));
		b = b >> 1;
		if (bit) { b = b | 0x80; };
		memory.Poke(address, b);
		setflag(z, b == 0);
		setflag(n, b >= 0x80);
	};

	void lsr_a(void)
	{
		setflag(c, (a & 0x01));
		a = a >> 1;
		setflag(z, a == 0);
		setflag(n, false);
	};

	void lsr(UINT16_T address)
	{
		UINT8_T b;
		b = memory.Peek(address);
		setflag(c, (b & 0x01));
		b = b >> 1;
		memory.Poke(address, b);
		setflag(z, b == 0);
		setflag(n, false);
	};

	void bit(UINT16_T address)
	{
		UINT8_T h;
		h = memory.Peek(address);
		setflag(n, (h & 0x80));
		setflag(v, (h & 0x40));
		setflag(z, (h & a) == 0);
	};

	void brk(void)
	{
		setflag(b, true);
		inc(1);
		push(hi(pc));
		push(lo(pc));
		//dec(1);
		push(p);
		setflag(i, true);
		pc = memory.PeekW(0xfffe);
	};

	void cmp(UINT16_T address)
	{
		UINT16_T h;
		h = a - memory.Peek(address);
		setflag(c, h <= 0xff);
		setflag(z, lo(h) == 0);
		setflag(n, lo(h) >= 0x80);
	};

	void cpx(UINT16_T address)
	{
		UINT16_T h;
		h = x - memory.Peek(address);
		setflag(c, h <= 0xff);
		setflag(z, lo(h) == 0);
		setflag(n, lo(h) >= 0x80);
	};

	void cpy(UINT16_T address)
	{
		UINT16_T h;
		h = y - memory.Peek(address);
		setflag(c, h <= 0xff);
		setflag(z, lo(h) == 0);
		setflag(n, lo(h) >= 0x80);
	};

	void dec_(UINT16_T address)
	{
		UINT16_T h;
		h = memory.Peek(address) - 1;
		memory.Poke(address, h);
		setflag(z, lo(h) == 0);
		setflag(n, lo(h) >= 0x80);
	};

	void dex(void)
	{
		UINT16_T h;
		h = x - 1;
		x = h;
		setflag(z, x == 0);
		setflag(n, x >= 0x80);
	};

	void dey(void)
	{
		UINT16_T h;
		h = y - 1;
		y = h;
		setflag(z, lo(h) == 0);
		setflag(n, lo(h) >= 0x80);
	};

	void eor(UINT16_T address)
	{
		a = a ^ memory.Peek(address);
		setflag(z, a == 0);
		setflag(n, a >= 0x80);
	};

	void inc_(UINT16_T address)
	{
		UINT16_T h;
		h = memory.Peek(address) + 1;
		memory.Poke(address, h);
		setflag(z, lo(h) == 0);
		setflag(n, lo(h) >= 0x80);
	};

	void inx(void)
	{
		UINT16_T h;
		h = x + 1;
		x = h;
		setflag(z, x == 0);
		setflag(n, x >= 0x80);
	};

	void iny(void)
	{
		UINT16_T h;
		h = y + 1;
		y = h;
		setflag(z, y == 0);
		setflag(n, y >= 0x80);
	};

	void jmp(UINT16_T address)
	{
		pc = address;
	};

	void jsr(UINT16_T address)
	{
		dec(1);
		push(hi(pc));
		push(lo(pc));
		pc = address;
	};

	void rts(void)
	{
		pc = pull();
		pc = pc + pull() * 256 + 1;
	};

	void rti(void)
	{
		p = pull();
		pc = pull();
		pc = pc + pull() * 256;
	};

	void lda(UINT16_T address)
	{
		a = memory.Peek(address);
		setflag(z, a == 0);
		setflag(n, a >= 0x80);
	};

	void ldx(UINT16_T address)
	{
		x = memory.Peek(address);
		setflag(z, x == 0);
		setflag(n, x >= 0x80);
	};

	void ldy(UINT16_T address)
	{
		y = memory.Peek(address);
		setflag(z, y == 0);
		setflag(n, y >= 0x80);
	};

	void pla(void)
	{
		a = pull();
		setflag(z, a == 0);
		setflag(n, a >= 0x80);
	};

	void sta(UINT16_T address)
	{
		memory.Poke(address, a);
	};

	void stx(UINT16_T address)
	{
		memory.Poke(address, x);
	};

	void sty(UINT16_T address)
	{
		memory.Poke(address, y);
	};

	void tax(void)
	{
		x = a;
		setflag(z, x == 0);
		setflag(n, x >= 0x80);
	};

	void tay(void)
	{
		y = a;
		setflag(z, y == 0);
		setflag(n, y >= 0x80);
	};

	void tsx(void)
	{
		x = s;
		setflag(z, x == 0);
		setflag(n, x >= 0x80);
	};

	void txa(void)
	{
		a = x;
		setflag(z, a == 0);
		setflag(n, a >= 0x80);
	};

	void txs(void)
	{
		s = x;
	};

	void tya(void)
	{
		a = y;
		setflag(z, a == 0);
		setflag(n, a >= 0x80);
	};

//****************************************************************************************************
// Interrupts and reset
//****************************************************************************************************
	void irq(void)
	{
		if (!flagset(i)) {
			setflag(b, false);
			push(hi(pc));
			push(lo(pc));
			push(p);
			setflag(i, true);
			pc = memory.PeekW(0xfffe);
			
			addkey();

			timer50hz += 20;
		}
	};

	void nmi(void)
	{
		setflag(b, false);
		push(hi(pc));
		push(lo(pc));
		push(p);
		setflag(i, true);
		pc = memory.PeekW(0xfffa);
	}

	void reset(void)
	{
		pc = memory.PeekW(0xfffc);
		setflag(i, true);
	}

public:
	MOS6510(void);
	Memory memory;
	void run(UINT16_T address);
	void dump(UINT8_T instr);
};

