#ifndef __KEYBOARD_HPP__
#define __KEYBOARD_HPP__

#include <cstdint>
#include "SDL.h"

#define NUM_OF_KEYPAD	16		// 16 key keypad

class Keyboard
{
public:
	Keyboard();
	uint8_t getNumOfKeys() { return NUM_OF_KEYPAD; };
	bool getKey(uint8_t key) { return m_Keypad[key]; };
	void processPress();

private:
	void initPress();

	bool m_Keypad[NUM_OF_KEYPAD];
};

#endif // __KEYBOARD_HPP__
