#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

#include "../include/Core/Chip8.h"
#include "../include/Graphics/graphics.h"

#include "SDL.h"


int main(int argc, char** argv) {

	if (argc != 2)
	{
		std::cout << "Enter a rom name!\n";
		return 1;
	}
	
	auto c8 = std::make_unique<Chip8>();
	auto displaymanager = std::make_unique<DisplayManager>();

	// exit program if the rom was not loaded
	if (!c8->load_rom(argv[1]))
	{
		std::cout << "Could not load the ROM. Exiting program";
		return -1;
	}

	// chip8 loop
	while (true)
	{
		c8->cycle();

		SDL_Event e;
		displaymanager->process_event(c8->keypad, e);

		if (c8->draw_flag)
		{
			displaymanager->draw(c8->display);
			c8->draw_flag = false;
		}

		// sleep to slow speed
		std::this_thread::sleep_for(std::chrono::microseconds(1100));
	}
}