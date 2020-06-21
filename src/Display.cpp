#include "Display.hpp"
#include <iostream>

Display::Display()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init video error: " << SDL_GetError() << std::endl;
	}
	else
	{
		m_Width = DISPLAY_WIDTH;
		m_Height = DISPLAY_HEIGHT;
		m_PixelSize = DISPLAY_PIXEL_SIZE;
		m_Window = SDL_CreateWindow("YACH8E - Yet Another Chip8 Emulator",
			100,
			100,
			m_Width * m_PixelSize,
			m_Height * m_PixelSize,
			SDL_WINDOW_SHOWN);
		if (m_Window == nullptr)
		{
			std::cout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
		}
		else
		{
			clear();
			m_Surface = SDL_CreateRGBSurfaceFrom(
				(void*)m_DisplayData,
				m_Width * m_PixelSize,
				m_Height * m_PixelSize,
				3 * 8,
				m_Width * m_PixelSize * 3,
				0xFF0000,	// R mask
				0x00FF00,	// G mask
				0x0000FF,	// B mask
				0x000000);	// A mask
		}
	}
}

void Display::render()
{
	SDL_BlitSurface(
		m_Surface,
		NULL,
		SDL_GetWindowSurface(m_Window),
		NULL
	);
	SDL_UpdateWindowSurface(m_Window);
}

void Display::putPixel(uint8_t row, uint8_t col, uint8_t val)
{	
	for (uint8_t idxY = 0; idxY < m_PixelSize; idxY++)
	{
		for (uint8_t idxX = 0; idxX < m_PixelSize; idxX++)
		{
			m_DisplayData[((row % DISPLAY_HEIGHT) * m_PixelSize) + idxX][((col % DISPLAY_WIDTH) * m_PixelSize) + idxY][1] ^= val;	// set only Green
		}
	}
}

uint8_t Display::getPixel(uint8_t row, uint8_t col)
{
	return m_DisplayData[(row % DISPLAY_HEIGHT) * m_PixelSize][(col % DISPLAY_WIDTH) * m_PixelSize][1];
}

void Display::clear()
{
	memset(m_DisplayData, 0, m_Width * m_PixelSize * m_Height * m_PixelSize * 3);
}
