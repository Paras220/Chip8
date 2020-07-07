#pragma once

#include "../../include/common.h"

#include "SDL.h"



class DisplayManager
{
public:
	DisplayManager();
	~DisplayManager();
	void draw(const std::array<uint8_t, 2048>& data);
	void process_event(std::array<Key, 16>&, SDL_Event& e);

private:
	const int width = 1024;
	const int height = 512;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* sdl_texture = nullptr;

	std::array<uint32_t, 64 * 32> pixels = {};

	std::array<int, 16> keymap = {
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
	SDLK_4,
	SDLK_r,
	SDLK_f,
	SDLK_v,
	};

};

