#include <algorithm> 
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "Chip8.h"

// DEBUG STATEMENTS
#include <iomanip>
template<typename T>
constexpr auto PRINT_OPCODE(T opcode) { std::cout << "Opcode: 0x" << std::hex << std::setw(4) << std::setfill('0') << opcode << std::uppercase << std::endl;; }
template<typename T>
constexpr auto PRINT_CASE(T switchcase) { std::cout << "Case: 0x" << std::hex << std::setw(4) << std::setfill('0') << switchcase << std::uppercase << std::endl;; }


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
	std::move(fontset.begin(), fontset.end(), memory.begin());
}

bool Chip8::LoadROM(const char* filename)
{
	bool loaded = true;

	auto path = std::filesystem::current_path() / "ROMs" / filename;
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
	opcode = (memory[pc] << 8) | memory[pc + 1];
	pc += 2;

	switch (opcode & 0xF000)
	{
		// 0 
		{
	case 0x0000:
		switch (opcode)
		{
		case 0x00E0:
			break;
		case 0x00EE:
			break;
		default:
			break;
		}
		break;
		}
		// 1-7
		{
	case 0x1000:
		PRINT_OPCODE(opcode);
		PRINT_CASE(0x1000);
		break;
	case 0x2000:
		PRINT_OPCODE(opcode);
		PRINT_CASE(0x0000);
		break;
	case 0x3000:
		PRINT_OPCODE(opcode);
		PRINT_CASE(0x0000);
		break;
	case 0x4000:
		PRINT_OPCODE(opcode);
		PRINT_CASE(0x0000);
		break;
	case 0x5000:
		PRINT_OPCODE(opcode);
		PRINT_CASE(0x0000);
		break;
	case 0x6000:
		PRINT_OPCODE(opcode);
		PRINT_CASE(0x0000);
		break;
	case 0x7000:
		PRINT_OPCODE(opcode);
		PRINT_CASE(0x0000);
		break;
		}
		// 8
		{
	case 0x8000:
		uint16_t lastHex = opcode & 0x000F;
		switch (lastHex)
		{
		case 0x0001:
			break;
		case 0x0002:
			break;
		case 0x0003:
			break;
		case 0x0004:
			break;
		case 0x0005:
			break;
		case 0x0006:
			break;
		case 0x0007:
			break;
		case 0x000E:
			break;
		default:
			std::cout << "Could not find opcode in case 0x8000\n";
			PRINT_OPCODE(opcode);
			break;
		}
		break;
		}
		// after 8
		{
	case 0x9000:
		PRINT_OPCODE(opcode);
		PRINT_CASE(0x0000);
		break;
	case 0xA000:
		PRINT_OPCODE(opcode);
		PRINT_CASE(0x0000);
		break;
	case 0xB000:
		PRINT_OPCODE(opcode);
		PRINT_CASE(0x0000);
		break;
	case 0xC000:
		PRINT_OPCODE(opcode);
		PRINT_CASE(0x0000);
		break;
	case 0xD000:
		PRINT_OPCODE(opcode);
		PRINT_CASE(0x0000);
		break;
		}

	case 0xE000:
		uint16_t thirdHex = opcode & 0x00F0;
		switch (thirdHex)
		{
		case 0x0090:
			break;
		case 0x00A0:
			break;
		default:
			std::cout << "Could not find opcode in case 0xE000\n";
			PRINT_OPCODE(opcode);
			break;
		}
		break;

	case 0xF000:
		uint16_t bottomHalf = opcode & 0x00FF;
		switch (bottomHalf)
		{
		case 0x0007:
			break;
		case 0x000A:
			break;
		case 0x0015:
			break;
		case 0x0018:
			break;
		case 0x001E:
			break;
		case 0x0029:
			break;
		case 0x0033:
			break;
		case 0x0055:
			break;
		case 0x0065:
			break;
		default:
			std::cout << "Could not find opcode in case 0xF000\n";
			PRINT_OPCODE(opcode);
			break;
		}
		break;

	default:
		std::cout << "Unknown opcode\n";
		PRINT_OPCODE(opcode);
	}
}