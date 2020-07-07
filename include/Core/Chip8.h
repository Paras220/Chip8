#pragma once
#include "../../include/common.h"

class Chip8 {
	static constexpr int PC_START = 0x200;
	static constexpr int WIDTH = 64;
	static constexpr int HEIGHT = 32;

public:
	Chip8();

	void cycle();
	bool load_rom(const char* filename);

	std::array<uint8_t, WIDTH * HEIGHT> display;
	std::array<Key, 16> keypad;

	bool draw_flag;

private:
	uint16_t pc;
	uint16_t opcode;

	uint16_t index_reg;
	std::array<uint8_t, 4096> memory = {};
	std::array<uint8_t, 16> V = {};
	std::array<uint16_t, 16> stack = {};
	uint8_t sp;

	uint8_t delay_timer;
	uint8_t sound_timer;
};
