// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
// http://mattmik.com/files/chip8/mastering/chip8.html

#include <iostream>
#include "Chip8.hpp"

int main(int argc, char* argv[])
{
	Chip8 chip8;

	if (argc > 1)
	{
        // perform chip8 initialization and if succeeded run endlessly
        if (chip8.init(argv[1]) == true)
        {
            chip8.run();
        }
	}
	else
	{
		std::cout << "Usage:" << std::endl;
		std::cout << "YACH8E.exe [filename]" << std::endl;
		return -1;
	}

	return 0;
}

#if 0
#include <chrono>
#include <thread>

#include "beeper.h"

void sleep(int duration) {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(duration)
    );
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_AUDIO);
    Beeper::open();
    Beeper::setVolume(1.0);
    Beeper::play();

    const double c4 = 261.63;
    const double e4 = 329.63;
    const double g4 = 392.00;

    Beeper::setFrequency(c4);
    sleep(5500);
    Beeper::setFrequency(e4);
    sleep(500);
    Beeper::setFrequency(g4);
    sleep(500);

    Beeper::stop();
    Beeper::close();
    SDL_Quit();
    return 0;
}

#endif