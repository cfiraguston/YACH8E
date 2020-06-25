#include "CPU.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>

CPU::CPU(Memory* RAM, Memory* STACK, Keyboard* Keyboard, Display* Display, Audio* Audio, uint16_t offsetProgram, uint16_t offsetFont)
{
	m_RAM = RAM;
	m_STACK = STACK;
	m_Keyboard = Keyboard;
	m_Display = Display;
	m_Audio = Audio;

	m_OffsetProgram = offsetProgram;
	m_OffsetFont = offsetFont;

	// initialize all registers to 0
	memset(m_RegisterV, 0, 0xF);
	m_RegisterI = 0u;
	m_RegisterPC = m_OffsetProgram;
	m_RegisterSP = 0u;
	m_RegisterDT = 0u;
	m_RegisterST = 0u;

	/* initialize random seed: */
	srand((unsigned int)time(NULL));
}

bool CPU::loadROM(char* filename, uint16_t offset)
{
	bool bRetVal = false;

	// open file
	std::ifstream streamInputFile(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (streamInputFile.is_open() == true)
	{
		// get file size
		std::ifstream::pos_type uiFileSize = streamInputFile.tellg();
		// read ROM data
		streamInputFile.seekg(0, std::ios::beg);
		streamInputFile.read((char*)(m_RAM->getMemory() + offset), uiFileSize);
		streamInputFile.close();
		// print some ROM info
		std::cout << "Loaded file " << filename << " succesfully." << std::endl;
		std::cout << "\tROM size = " << uiFileSize << " bytes." << std::endl;
		bRetVal = true;
	}
	else
	{
		std::cout << "Error opening file " << filename << std::endl;
		bRetVal = false;
	}

	return bRetVal;
}

bool CPU::loadROM(uint8_t* data, uint16_t offset, size_t size)
{
	memcpy((char*)(m_RAM->getMemory() + offset), data, size);
	return true;
}

void CPU::execute()
{
	// update delay and sound timers
	updateTimers();

	// fetch next instruction
	uint16_t uiCurrentInstruction = m_RAM->read16(m_RegisterPC);

	//printf("PC=0x%04X, OP=0x%04X\n", m_RegisterPC, uiCurrentInstruction);

	// breakdown of instruction
	uint16_t nnn = uiCurrentInstruction & 0x0FFF;
	uint8_t n = uiCurrentInstruction & 0x000F;
	uint8_t nn = uiCurrentInstruction & 0x00FF;
	uint8_t x = (uiCurrentInstruction & 0x0F00) >> 8;
	uint8_t y = (uiCurrentInstruction & 0x00F0) >> 4;
	uint8_t kk = uiCurrentInstruction & 0x00FF;

	// decode instruction
	switch ((uiCurrentInstruction >> 12) & 0x000F)
	{
	case 0x0:
		switch (uiCurrentInstruction & 0x0FFF)
		{
		case 0x0E0:	// 00E0 - CLS
			// Clear the display.
			CLS();
			break;
		case 0xEE:	// 00EE - RET
			// Return from a subroutine.
			RET();
			break;
		default:	// SYS addr
			SYS();
			break;
		}
		break;
	case 0x1:		// 1nnn - JP addr
		// Jump to location nnn.
		JP(nnn, 0);
		break;
	case 0x2:		// 2nnn - CALL addr
		// Call subroutine at nnn.
		CALL(nnn);
		break;
	case 0x3:		// 3xkk - SE Vx, byte
		// Skip next instruction if Vx = kk.
		SE(m_RegisterV[x], kk);
		break;
	case 0x4:		// 4xkk - SNE Vx, byte
		// Skip next instruction if Vx != kk.
		SNE(m_RegisterV[x], kk);
		break;
	case 0x5:		// 5xy0 - SE Vx, Vy
		// Skip next instruction if Vx = Vy.
		SE(m_RegisterV[x], m_RegisterV[y]);
		break;
	case 0x6:		// 6xkk - LD Vx, byte
		// Set Vx = kk.
		LD(&(m_RegisterV[x]), &kk);
		break;
	case 0x7:		// 7xkk - ADD Vx, byte
		// Set Vx = Vx + kk.
		ADD_NoCarry(&(m_RegisterV[x]), &kk);
		break;
	case 0x8:
		switch (n)
		{
		case 0x0:	// 8xy0 - LD Vx, Vy
			// Set Vx = Vy.
			LD(&(m_RegisterV[x]), &(m_RegisterV[y]));
			break;
		case 0x1:	// 8xy1 - OR Vx, Vy
			// Set Vx = Vx OR Vy.
			OR(&(m_RegisterV[x]), &(m_RegisterV[y]));
			break;
		case 0x2:	// 8xy2 - AND Vx, Vy
			// Set Vx = Vx AND Vy.
			AND(&(m_RegisterV[x]), &(m_RegisterV[y]));
			break;
		case 0x3:	// 8xy3 - XOR Vx, Vy
			// Set Vx = Vx XOR Vy.
			XOR(&(m_RegisterV[x]), &(m_RegisterV[y]));
			break;
		case 0x4:	// 8xy4 - ADD Vx, Vy
			// Set Vx = Vx + Vy, set VF = carry.
			ADD(&(m_RegisterV[x]), &(m_RegisterV[y]));
			break;
		case 0x5:	// 8xy5 - SUB Vx, Vy
			// Set Vx = Vx - Vy, set VF = NOT borrow.
			SUB(&(m_RegisterV[x]), &(m_RegisterV[y]));
			break;
		case 0x6:	// 8xy6 - SHR Vx
			// Set Vx = Vy SHR 1.
			SHR(&(m_RegisterV[x]));
			break;
		case 0x7:	// 8xy7 - SUBN Vx, Vy
			// Set Vx = Vy - Vx, set VF = NOT borrow.
			SUBN(&(m_RegisterV[x]), &(m_RegisterV[y]));
			break;
		case 0xE:	// 8xyE - SHL Vx
			// Set Vx = Vx SHL 1.
			SHL(&(m_RegisterV[x]));
			break;
		}
		break;
	case 0x9:		// 9xy0 - SNE Vx, Vy
		// Skip next instruction if Vx != Vy.
		SNE(m_RegisterV[x], m_RegisterV[y]);
		break;
	case 0xA:		// Annn - LD I, addr
		// Set I = nnn.
		LD(&m_RegisterI, (uint16_t *)(&nnn));
		break;
	case 0xB:		// Bnnn - JP V0, addr
		// Jump to location nnn + V0.
		JP(nnn, m_RegisterV[0]);
		break;
	case 0xC:		// Cxkk - RND Vx, byte
		// Set Vx = random byte AND kk.
		RND(&(m_RegisterV[x]), &kk);
		break;
	case 0xD:		// Dxyn - DRW Vx, Vy, nibble
		// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
		DRW(m_RegisterV[x], m_RegisterV[y], n);
		m_RegisterPC += 2;
		break;
	case 0xE:
		switch (nn)
		{
		case 0x9E:	// Ex9E - SKP Vx
			// Skip next instruction if key with the value of Vx is pressed.
			SKP(m_RegisterV[x]);
			break;
		case 0xA1:	// ExA1 - SKNP Vx
			// Skip next instruction if key with the value of Vx is not pressed.
			SKNP(m_RegisterV[x]);
			break;
		default:
			printf("Incorrect opcode: 0x%04X\n", uiCurrentInstruction);
		}
		break;
	case 0xF:
		switch (nn)
		{
		case 0x07:	// Fx07 - LD Vx, DT
			// Set Vx = delay timer value.
			LD(&(m_RegisterV[x]), &m_RegisterDT);
			break;
		case 0x0A: // Fx0A - LD Vx, K
			for (uint8_t idx = 0; idx < m_Keyboard->getNumOfKeys(); idx++)
			{
				if (m_Keyboard->getKey(idx) == true)
				{
					m_RegisterV[x] = idx;
					m_RegisterPC += 2;
					break;
				}
			}
			break;
		case 0x15:	// Fx15 - LD DT, Vx
			// Set delay timer = Vx.
			LD(&m_RegisterDT, &(m_RegisterV[x]));
			break;
		case 0x18:	// Fx18 - LD ST, Vx
			// Set sound timer = Vx.
			LD(&m_RegisterST, &(m_RegisterV[x]));
			m_Audio->setBeep(m_RegisterST);
			break;
		case 0x1E:	// Fx1E - ADD I, Vx
			// Set I = I + Vx.
			ADD_NoCarry(&m_RegisterI, &(m_RegisterV[x]));
			break;
		case 0x29:	// Fx29 - LD F, Vx
			// Set I = location of sprite for digit Vx.
			{
				uint16_t offset = m_OffsetFont + (m_RegisterV[x] * 5);
				LD(&m_RegisterI, &offset);
			}
			break;
		case 0x33:	// Fx33 - LD B, Vx
			// Store BCD representation of Vx in memory locations I, I+1, and I+2.
			m_RAM->write8(m_RegisterI, (m_RegisterV[x] / 100));
			m_RAM->write8(m_RegisterI + 1, ((m_RegisterV[x] / 10) % 10));
			m_RAM->write8(m_RegisterI + 2, (m_RegisterV[x] % 10));
			m_RegisterPC += 2;
			break;
		case 0x55:	// Fx55 - LD [I], Vx
			// Store registers V0 through Vx in memory starting at location I.
			for (uint8_t idx = 0; idx <= x; idx++)
			{
				// use direct memory access instead of LD command for not incrementing PC
				m_RAM->write8(m_RegisterI + idx, m_RegisterV[idx]);
			}
			//m_RegisterI = m_RegisterI + x + 1;
			m_RegisterPC += 2;
			break;
		case 0x65:	// Fx65 - LD Vx, [I]
			// Read registers V0 through Vx from memory starting at location I.
			for (uint8_t idx = 0; idx <= x; idx++)
			{
				// use direct memory access instead of LD command for not incrementing PC
				m_RegisterV[idx] = m_RAM->read8(m_RegisterI + idx);
			}
			//m_RegisterI = m_RegisterI + x + 1;
			m_RegisterPC += 2;
			break;
		default:
			printf("Incorrect opcode: 0x%04X\n", uiCurrentInstruction);
		}		
		break;
	}
}

void CPU::SYS()
{
	printf("Unimplemented SYS\n");
	m_RegisterPC += 2;
}

// Clear the display.
void CPU::CLS()
{
	m_Display->clear();
	m_RegisterPC += 2;
}

// Return from a subroutine.
void CPU::RET()
{
	m_RegisterPC = m_STACK->read16(m_RegisterSP);
	m_RegisterPC += 2;
	m_RegisterSP -= 2;
}

// Jump to location at base + address.
void CPU::JP(uint16_t address, uint8_t base)
{
	m_RegisterPC = base + address;
}

// Call subroutine at address.
void CPU::CALL(uint16_t address)
{
	m_RegisterSP += 2;
	m_STACK->write16(m_RegisterSP, m_RegisterPC);
	m_RegisterPC = address;
}

// Skip next instruction if val1 = val2.
void CPU::SE(uint8_t val1, uint8_t val2)
{
	if (val1 == val2)
	{
		m_RegisterPC += 4;
	}
	else
	{
		m_RegisterPC += 2;
	}
}

// Skip next instruction if val1 != val2.
void CPU::SNE(uint8_t val1, uint8_t val2)
{
	if (val1 != val2)
	{
		m_RegisterPC += 4;
	}
	else
	{
		m_RegisterPC += 2;
	}
}

// Set 8-bits dst = src.
void CPU::LD(uint8_t *dst, uint8_t *src)
{
	*dst = *src;
	m_RegisterPC += 2;
}

// Set 16-bits dst = src.
void CPU::LD(uint16_t* dst, uint16_t* src)
{
	*dst = *src;
	m_RegisterPC += 2;
}

// Set dst = dst + src, VF = 1 if carry, VF = 0 if no carry
void CPU::ADD(uint8_t* dst, uint8_t* src)
{
	*dst = (*dst) + (*src);
	m_RegisterV[0xF] = (((uint16_t)(*dst) + (uint16_t)(*src)) > 255) ? 0x01 : 0x00;
	m_RegisterPC += 2;
}

// Set 8-bits dst = dst + src.
void CPU::ADD_NoCarry(uint8_t* dst, uint8_t* src)
{
	*dst = (*dst) + (*src);
	m_RegisterPC += 2;
}

// Set 16-bits dst = dst + src.
void CPU::ADD_NoCarry(uint16_t* dst, uint8_t* src)
{
	*dst = (*dst) + (*src);
	m_RegisterPC += 2;
}

// Set dst = dst OR src.
void CPU::OR(uint8_t* dst, uint8_t* src)
{
	*dst = (*dst) | (*src);
	m_RegisterPC += 2;
}

// Set dst = dst AND src.
void CPU::AND(uint8_t* dst, uint8_t* src)
{
	*dst = (*dst) & (*src);
	m_RegisterPC += 2;
}

// Set dst = dst XOR src.
void CPU::XOR(uint8_t* dst, uint8_t* src)
{
	*dst = (*dst) ^ (*src);
	m_RegisterPC += 2;
}

// Set dst = dst - src, VF = 1 if no borrow, VF = 0 if borrow.
void CPU::SUB(uint8_t* dst, uint8_t* src)
{
	m_RegisterV[0xF] = ((*dst) >= (*src)) ? 0x01 : 0x00;
	*dst = (*dst) - (*src);
	m_RegisterPC += 2;
}

// Set dst shift right by 1, VF = LSB before shift.
void CPU::SHR(uint8_t* dst)
{
	m_RegisterV[0xF] = (*dst) & 0x01;
	*dst = (*dst) >> 1;
	m_RegisterPC += 2;
}

// set dst = src - dst, VF = 1 if no borrow, VF = 0 if borrow.
void CPU::SUBN(uint8_t* dst, uint8_t* src)
{
	*dst = (*src) - (*dst);
	m_RegisterV[0xF] = ((*src) >= (*dst)) ? 0x01 : 0x00;
	m_RegisterPC += 2;
}

// Set dst shift left by 1, VF = MSB before shift.
void CPU::SHL(uint8_t* dst)
{
	m_RegisterV[0xF] = ((*dst) & 0x80) >> 7;
	*dst = (*dst) << 1;
	m_RegisterPC += 2;
}

// dst = random[0..255] AND src
void CPU::RND(uint8_t* dst, uint8_t* src)
{
	*dst = (rand() % 256) & (*src);
	m_RegisterPC += 2;
}

// Draw sprite at position [col, row]
void CPU::DRW(uint8_t col, uint8_t row, uint8_t size)
{
	m_RegisterV[0xF] = 0x00;
	for (uint8_t idxY = 0; idxY < size; idxY++)								// go over all bytes composing the sprite
	{
		uint8_t CurrentSpriteByte = m_RAM->read8(m_RegisterI + idxY);		// read current byte of sprite
		for (int8_t idxX = 0; idxX < 8; idxX++)								// go over all bits (pixels) in byte
		{
			if ((CurrentSpriteByte & (0x80 >> idxX)) != 0)					//	if current bit is on
			{
				if (m_Display->getPixel(row + idxY, col + idxX) == 0xFF)	// if pixel is already on
				{
					m_RegisterV[0xF] = 0x01;								// set VF = 1 since pixel have XOR logic (pixel_on XOR current_on == 0 (unset))
				}
				m_Display->putPixel(row + idxY, col + idxX, 0xFF);
			}
		}
	}
}

// Skip next instruction if key(val) is pressed, else proceed to next instruction.
void CPU::SKP(uint8_t val)
{
	if (m_Keyboard->getKey(val) == true)
	{
		m_RegisterPC += 4;
	}
	else
	{
		m_RegisterPC += 2;
	}
}

// Skip next instruction if key(val) is not pressed, else proceed to next instruction.
void CPU::SKNP(uint8_t val)
{
	if (m_Keyboard->getKey(val) == false)
	{
		m_RegisterPC += 4;
	}
	else
	{
		m_RegisterPC += 2;
	}
}

// Update delay timer and sound timer registers
void CPU::updateTimers()
{
	if (m_RegisterDT > 0)
	{
		m_RegisterDT--;
	}
	if (m_RegisterST > 0)
	{
		m_RegisterST--;
	}
}
