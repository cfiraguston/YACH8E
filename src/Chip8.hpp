#ifndef __CHIP8_HPP__
#define __CHIP8_HPP__

#include "Memory.hpp"
#include "Keyboard.hpp"
#include "Display.hpp"
#include "Audio.hpp"
#include "CPU.hpp"

#define MEMORY_RAM_SIZE				0x1000	// total of 4096 bytes
#define MEMORY_STACK_SIZE			0x20	// total 32 bytes = 16 16-bits addresses values

#define ADDRESS_START_OF_FONT		0x0		// Fonts [0-9A-F] starts at offset 0
#define ADDRESS_START_OF_PROGRAM	0x200	// 512 bytes offset start of program

class Chip8
{
public:
	Chip8();
	bool init(char* filename);
	void run();
private:
	Memory* m_RAM;
	Memory* m_STACK;
	Keyboard* m_Keyboard;
	Display* m_Display;
	Audio* m_Audio;
	CPU * m_CPU;
};

#endif // __CHIP8_HPP__
