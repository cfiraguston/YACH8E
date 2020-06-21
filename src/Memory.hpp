#ifndef __MEMORY_HPP__
#define __MEMORY_HPP__

#include <cstdint>

class Memory
{
public:
	Memory(uint16_t size);
	uint8_t read8(uint16_t address);
	void write8(uint16_t address, uint8_t value);
	uint8_t& operator[](int i) { return m_Memory[i]; };
	uint16_t read16(uint16_t address);
	void write16(uint16_t address, uint16_t value);
	uint8_t* getMemory() { return m_Memory; };
private:
	uint8_t *m_Memory;
	uint16_t m_Size;
};

#endif // __MEMORY_HPP__
