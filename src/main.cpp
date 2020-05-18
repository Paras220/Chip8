#include <iostream>

#include "Core/Chip8.h"


int main(int argc, char *argv[])
{
	Chip8 c8;

	if (!c8.LoadROM("invaders.c8")) {
		std::cout << "Could not load the ROM. Exiting program";
		return -1;
	}

	c8.Cycle();
}
