#include "Keyboard.hpp"

uint8_t Chip8KeyMap[NUM_OF_KEYPAD] = {
	SDLK_x,
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_q,
	SDLK_w,
	SDLK_e,
	SDLK_a,
	SDLK_s,
	SDLK_d,
	SDLK_z,
	SDLK_c,
	SDLK_r,
	SDLK_f,
	SDLK_v
};

Keyboard::Keyboard()
{
	initPress();
}

void Keyboard::processPress()
{
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				exit(0);
			}

			for (uint8_t idxKey = 0; idxKey < NUM_OF_KEYPAD; idxKey++)
			{
				if (event.key.keysym.sym == Chip8KeyMap[idxKey])
				{
					m_Keypad[idxKey] = true;
				}
			}
			break;
		case SDL_KEYUP:
			for (uint8_t idxKey = 0; idxKey < NUM_OF_KEYPAD; idxKey++)
			{
				if (event.key.keysym.sym == Chip8KeyMap[idxKey])
				{
					m_Keypad[idxKey] = false;
				}
			}
			break;
		}
	}
}

void Keyboard::initPress()
{
	for (uint8_t idxKey = 0; idxKey < NUM_OF_KEYPAD; idxKey++)
	{
		m_Keypad[idxKey] = false;
	}
}
