#pragma once

#include <cstdint>
#include "SDL.h"

#define DISPLAY_WIDTH		64
#define DISPLAY_HEIGHT		32
#define DISPLAY_PIXEL_SIZE	8

class Display
{
public:
	Display();
	void render();
	void putPixel(uint8_t row, uint8_t col, uint8_t val);
	uint8_t getPixel(uint8_t row, uint8_t col);
	void clear();

private:
	uint8_t m_Width;
	uint8_t m_Height;
	uint8_t m_PixelSize;
	uint8_t m_DisplayData[DISPLAY_HEIGHT * DISPLAY_PIXEL_SIZE][DISPLAY_WIDTH * DISPLAY_PIXEL_SIZE][3] = { 0 };
	SDL_Window* m_Window;
	SDL_Surface* m_Surface;
};

