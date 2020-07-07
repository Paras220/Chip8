#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "../../include/Core/Chip8.h"


// DEBUG STATEMENTS
#include <iomanip>
template<typename T>
constexpr auto PRINT_OPCODE(T code) { std::cout << "opcode: 0x" << std::hex << std::setw(4) << std::setfill('0') << code << std::uppercase << std::endl;; }

// Macro to parse parts of the opcode
#define NNN (opcode & 0x0FFF)
#define NN (opcode & 0x00FF)
#define N (opcode & 0x000F)
// >> operator takes precedence over & so use brackets
#define X ((opcode & 0x0F00) >> 8)
#define Y ((opcode & 0x00F0) >> 4)


Chip8::Chip8()
{
	pc = PC_START;
	opcode = 0;
	index_reg = 0;

	draw_flag = false;

	sp = 0;
	delay_timer, sound_timer = 60;

	keypad.fill(Key::NotPressed);
	display.fill(0);

	// Load the fontset at location 0x000 in memory
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

// Initialise and load ROM into memory
bool Chip8::load_rom(const char* filename)
{
	bool loaded = true;

	auto pathToRom = std::filesystem::current_path() / "ROMs" / filename;
	std::ifstream rom(pathToRom, std::ios::binary);
	if (rom.is_open())
	{
		rom.read(reinterpret_cast<char*>(memory.data()) + PC_START, std::filesystem::file_size(pathToRom));
	}
	else
	{
		loaded = false;
	}

	return loaded;
}


void Chip8::cycle()
{
	opcode = (memory[pc] << 8) | memory[pc + 1];
	PRINT_OPCODE(opcode);

	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode)
		{
		case 0x00E0:
			display.fill(0);
			pc += 2;
			draw_flag = true;
			break;

		case 0x00EE:
			sp--;
			pc = stack[sp];
			pc += 2;
			break;

			// default case in case there was an error in finding the opcode
		default:
			std::cout << "Could not find opcode in case 0x0000\n";
			PRINT_OPCODE(opcode);
			exit(3);
		}
		break;

	case 0x1000:
		pc = NNN;
		break;

	case 0x2000:
		// the current pc is stored in stack[i] while the new pc uses stack[i+1]
		stack[sp] = pc;
		sp++;
		pc = NNN;
		break;

	case 0x3000:
		// skip the next instruction (pc += 4) if V[X] == NN else increment normally by 2
		pc += (V[X] == NN ? 4 : 2);
		break;

	case 0x4000:
		// skip the next instruction (pc += 4) if V[X] != NN else increment normally by 2
		pc += (V[X] != NN ? 4 : 2);
		break;

	case 0x5000:
		// skip the next instruction (pc += 4) if V[X] == V[Y] else increment normally by 2
		pc += (V[X] == V[Y] ? 4 : 2);
		break;

	case 0x6000:
		V[X] = NN;
		pc += 2;
		break;

	case 0x7000:
		V[X] += NN;
		pc += 2;
		break;

	case 0x8000:
		switch (N)
		{
		case 0x0000:
			V[X] = V[Y];
			pc += 2;
			break;

		case 0x0001:
			V[X] |= V[Y];
			pc += 2;
			break;

		case 0x0002:
			V[X] &= V[Y];
			pc += 2;
			break;

		case 0x0003:
			V[X] ^= V[Y];
			pc += 2;
			break;

		case 0x0004:
			//max of an 8 bit register is 255. set VF to 1 if there is a carry else and 0 if not
			V[0xF] = (V[X] + V[Y] > 255) ? 1 : 0;
			V[X] += V[Y];
			pc += 2;
			break;

		case 0x0005:
			// x = x - y. if y > x then there has to be a borrow for x. set VF to 0 if there was a borrow
			V[0xF] = (V[Y] > V[X]) ? 0 : 1;
			V[X] -= V[Y];
			pc += 2;
			break;

		case 0x0006:
			V[0xF] = V[X] & 0x01;
			V[X] >>= 1;
			pc += 2;
			break;

		case 0x0007:
			// x = y - x. If x > y there is a borrow for y
			V[0xF] = (V[X] > V[Y]) ? 0 : 1;
			V[X] = V[Y] - V[X];
			pc += 2;
			break;

		case 0x000E:
			V[0xF] = (V[X] & 0xF0) >> 7;
			V[X] <<= 1;
			pc += 2;
			break;

		default:
			std::cout << "Could not find opcode in case 0x8000\n";
			PRINT_OPCODE(opcode);
			exit(3);
		}
		break;

	case 0x9000:
		pc += (V[X] != V[Y]) ? 4 : 2;
		break;

	case 0xA000:
		index_reg = NNN;
		pc += 2;
		break;

	case 0xB000:
		pc = V[0x0] + NNN;
		break;

	case 0xC000:
	{
		// creates a random number between 0 and 255 using the random library
		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_int_distribution<int> dist(0, 255);

		V[X] = dist(generator) & NN;
		pc += 2;
	}
	break;

	case 0xD000:
	{
		int xCoord = V[X];
		int yCoord = V[Y];
		uint16_t height = N;

		V[0xF] = 0;
		for (size_t row = 0; row < height; row++)
		{
			int pixel = memory[index_reg + row];
			for (size_t col = 0; col < 8; col++)
			{
				if ((pixel & (0x80 >> col)) != 0)
				{
					if (display[(xCoord + col) + ((yCoord + row) * WIDTH)] == 1) { V[0xF] = 1; }
					display[(xCoord + col) + ((yCoord + row) * WIDTH)] ^= 1;
				}
			}
		}
	}
	draw_flag = true;
	pc += 2;
	break;

	case 0xE000:
		switch (Y)
		{
		case 0x0009:
			pc += (keypad[V[X]] == Key::Pressed) ? 4 : 2;
			break;

		case 0x000A:
			pc += (keypad[V[X]] == Key::NotPressed) ? 4 : 2;
			break;

		default:
			std::cout << "Could not find opcode in case 0xE000\n";
			PRINT_OPCODE(opcode);
			exit(3);
		}
		break;

	case 0xF000:
		switch (NN)
		{
		case 0x0007:
			V[X] = delay_timer;
			pc += 2;
			break;

		case 0x000A:
		{
			// find the enum in the keypad array. if not detected it will point to keypad.end()
			auto it = std::find(keypad.begin(), keypad.end(), Key::Pressed);
			if (it != keypad.end())
			{
				// stores the index position in VX
				V[X] = std::distance(keypad.begin(), it);
			}
			// return if there was no keypress and do not increment pc
			// imitates a loop running the same opcode until a keypress is detected
			else { return; }
		}
		pc += 2;
		break;

		case 0x0015:
			delay_timer = V[X];
			pc += 2;
			break;

		case 0x0018:
			sound_timer = V[X];
			pc += 2;
			break;

		case 0x001E:
			index_reg += V[X];
			pc += 2;
			break;

		case 0x0029:
			// Get the char from V[X] and look for that in memory. Fontset starts at 0x000. Find the char's offset by multiplying each char's size (5) with the reg value.
			index_reg = V[X] * 5;
			pc += 2;
			break;

		case 0x0033:
		{
			uint8_t onesCol = V[X] % 10;
			uint8_t tensCol = V[X] / 10 % 10;
			uint8_t hundredsCol = V[X] / 100 % 10;

			memory[index_reg] = hundredsCol;
			memory[index_reg + 1] = tensCol;
			memory[index_reg + 2] = onesCol;
		}
		pc += 2;
		break;

		case 0x0055:
			for (int i = 0; i <= X; i++)
			{
				memory[index_reg + i] = V[i];
			}
			pc += 2;
			break;

		case 0x0065:
			for (int i = 0; i <= X; i++)
			{
				V[i] = memory[index_reg + i];
			}
			pc += 2;
			break;

		default:
			std::cout << "Could not find opcode in case 0xF000\n";
			PRINT_OPCODE(opcode);
			exit(3);
		}
		break;

	default:
		std::cout << "Unknown opcode\n";
		PRINT_OPCODE(opcode);
		exit(3);
	}
	if (delay_timer > 0) delay_timer--;
	if (sound_timer > 0) sound_timer--;
}