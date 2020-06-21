#pragma once

#include <cstdint>
#include "Memory.hpp"
#include "Keyboard.hpp"
#include "Display.hpp"
#include "Audio.hpp"

class CPU
{
public:
	CPU(Memory* RAM, Memory* STACK, Keyboard* Keyboard, Display* Display, Audio* Audio, uint16_t offsetProgram, uint16_t offsetFont);
	void loadROM(char* filename, uint16_t offset);
	void loadROM(uint8_t* data, uint16_t offset, size_t size);
	void execute();

private:
	void updateTimers();
	void SYS();
	void CLS();
	void RET();
	void JP(uint16_t address, uint8_t base);
	void CALL(uint16_t address);
	void SE(uint8_t val1, uint8_t val2);
	void SNE(uint8_t val1, uint8_t val2);
	void LD(uint8_t* dst, uint8_t* src);
	void LD(uint16_t* dst, uint16_t* src);
	void ADD(uint8_t* dst, uint8_t* src);
	void ADD_NoCarry(uint8_t* dst, uint8_t* src);
	void ADD_NoCarry(uint16_t* dst, uint8_t* src);
	void OR(uint8_t* dst, uint8_t* src);
	void AND(uint8_t* dst, uint8_t* src);
	void XOR(uint8_t* dst, uint8_t* src);
	void SUB(uint8_t* dst, uint8_t* src);
	void SHR(uint8_t* dst);
	void SUBN(uint8_t* dst, uint8_t* src);
	void SHL(uint8_t* dst);
	void RND(uint8_t* dst, uint8_t* src);
	void DRW(uint8_t col, uint8_t row, uint8_t size);
	void SKP(uint8_t val);
	void SKNP(uint8_t val);

	Memory* m_RAM;
	Memory* m_STACK;
	Keyboard* m_Keyboard;
	Display* m_Display;
	Audio* m_Audio;
	uint8_t m_RegisterV[16];		// 16 general purpose registers
	uint16_t m_RegisterI;
	uint16_t m_RegisterPC;			// Program Counter register
	uint8_t m_RegisterSP;			// Stack Pointer register
	uint8_t m_RegisterDT;			// Delay Timer register
	uint8_t m_RegisterST;			// Sound Timer register
	uint16_t m_OffsetProgram;		// Start of program offset
	uint16_t m_OffsetFont;			// Fonts offset
};

