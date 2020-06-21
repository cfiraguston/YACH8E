#include "Memory.hpp"
#include <string>

Memory::Memory(uint16_t size)
{
	m_Memory = new uint8_t[size];
	memset(m_Memory, 0, size);	// initialize memory to all 0
	m_Size = size;
}

uint8_t Memory::read8(uint16_t address)
{
	return m_Memory[address];
}

void Memory::write8(uint16_t address, uint8_t value)
{
	m_Memory[address] = value;
}

uint16_t Memory::read16(uint16_t address)
{
	uint16_t uiRetVal = 0;
	// data is stored in memory as big-endian
	uiRetVal = (m_Memory[address] << 8) | (m_Memory[address + 1]);
	return uiRetVal;
}

void Memory::write16(uint16_t address, uint16_t value)
{
	m_Memory[address] = (value & 0xFF00) >> 8;
	m_Memory[address + 1] = (value & 0x00FF);
}
