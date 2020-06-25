// Chip-8 Emulator - Created by Cfir Aguston
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
