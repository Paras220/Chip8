#include <algorithm> 
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "Chip8.h"


Chip8::Chip8()
{
	pc = PC_START;
	opcode = 0;
	registerI = 0;

	sp = 0;
	delayTimer, soundTimer = 60;

	// Load the fontset at location 0x000 of memory
	std::array<uint8_t, 80> fontset = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, //0
	0x20, 0x60, 0x20, 0x20, 0x70, //1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	0x90, 0x90, 0xF0, 0x10, 0x10, //4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	0xF0, 0x10, 0x20, 0x40, 0x40, //7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	0xF0, 0x90, 0xF0, 0x90, 0x90, //A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	0xE0, 0x90, 0x90, 0x90, 0xE0, //D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	0xF0, 0x80, 0xF0, 0x80, 0x80  //F
	};
	std::copy(fontset.begin(), fontset.end(), memory.begin());
}

bool Chip8::LoadROM(const char* filename)
{
	bool loaded = true;

	std::filesystem::path path = std::filesystem::current_path() / "ROMs" / filename;
	std::ifstream rom(path, std::ios::binary);
	if (rom.is_open())
	{
		rom.read(reinterpret_cast<char*>(memory.data()) + PC_START, std::filesystem::file_size(path));
	}
	else {
		loaded = false;
	}

	return loaded;
}

void Chip8::Cycle()
{
}