#include "Audio.hpp"
#include <iostream>
#include "SDL.h"
#include "beeper.hpp"

Audio::Audio()
{
    if (SDL_Init(SDL_INIT_AUDIO) != 0)
    {
        std::cout << "SDL_Init audio error: " << SDL_GetError() << std::endl;
    }
    Beeper::open();
    Beeper::setVolume(0.5);
    //Beeper::play();
    Beeper::setFrequency(261.63);
    m_Ticks = 0;
}

void Audio::play()
{
    std::chrono::steady_clock::time_point CurrentTime = std::chrono::high_resolution_clock::now();
    long long DiffTime = std::chrono::duration_cast<std::chrono::microseconds>(CurrentTime - m_StartTime).count();
    if (DiffTime <= ((float)m_Ticks * 100.0 * 16.6))
    {
        Beeper::play();
    }
    else
    {
        Beeper::stop();
    }
}

void Audio::setBeep(uint8_t ticks)
{
    m_Ticks = ticks;
    m_StartTime = std::chrono::high_resolution_clock::now();
}
