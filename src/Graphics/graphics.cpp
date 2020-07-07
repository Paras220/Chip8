#include <iostream>
#include <iterator>
#include <algorithm>

#include "../../include/Graphics/Graphics.h"

//check renderer options
DisplayManager::DisplayManager()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Could not initialize SDL. Error: "<< SDL_GetError();
		exit(3);
	}

	window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{

		std::cout << "Could not create window. Error: " << SDL_GetError();
		exit(3);
	}
	std::cout << "Window created!\n";

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{

		std::cout << "Could not create renderer. Error: " << SDL_GetError();
		exit(3);
	}
	std::cout << "Renderer created!\n";

	// Create texture that stores frame buffer
	sdl_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
	if (sdl_texture == nullptr)
	{

		std::cout << "Could not create texture. Error: " << SDL_GetError();
		exit(3);
	}
	std::cout << "texture created!\n";
}

DisplayManager::~DisplayManager()
{
	if (sdl_texture != nullptr)
	{
		SDL_DestroyTexture(sdl_texture);
	}
	if (renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}
	if (window != nullptr)
	{
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}

void DisplayManager::draw(const std::array<uint8_t, 2048>& data)
{
	// copy the chip8 display array to our u32 array. store as 0xFFFFFFFF if value was non-zero (pixel on)
	std::transform(data.begin(), data.end(), pixels.begin(),
				   [](auto i)
				   {
					   return (i != 0) ? 0xFFFFFFFF : 0;
				   }
);

	// update SDL texture
	SDL_UpdateTexture(sdl_texture, NULL, &pixels, 64 * sizeof(uint32_t));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, sdl_texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void DisplayManager::process_event(std::array<Key, 16>& keypad, SDL_Event& e)
{
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) { exit(0); }

		// find which keydown enum was used
		if (e.type == SDL_KEYDOWN) {
			auto it = std::find(keymap.begin(), keymap.end(), e.key.keysym.sym);
			auto index = std::distance(keymap.begin(), it);
			keypad[index] = Key::Pressed;
		}

		// find which keydup enum was used
		if (e.type == SDL_KEYUP) {
			auto it = std::find(keymap.begin(), keymap.end(), e.key.keysym.sym);
			auto index = std::distance(keymap.begin(), it);
			keypad[index] = Key::NotPressed;
		}
	}
}
