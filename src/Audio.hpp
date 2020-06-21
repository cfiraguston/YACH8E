#pragma once

#include <cstdint>
#include <chrono>

class Audio
{
public:
	Audio();
	void play();
    void setBeep(uint8_t ticks);

private:
	uint8_t m_Ticks;
	std::chrono::steady_clock::time_point m_StartTime;
};

