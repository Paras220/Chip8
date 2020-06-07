#pragma once

#include <array>
#include <cstdint>


// TODO
// Opcodes
// SDL2 output

const int PC_START = 0x200;


class Chip8 {

enum class Key : uint8_t {
    NotPressed,
    Pressed 
};

public:
    Chip8();
	void Cycle();
	bool LoadROM(const char* filename);

    std::array<uint8_t, 64 * 32> display = {};
    std::array<Key, 16> keypad = {};

    bool drawFlag;
private:
    uint16_t pc;
    uint16_t opcode;
    uint16_t registerI;

    std::array<uint8_t, 4096> memory = {};
    std::array<uint8_t, 16> V = {};
    std::array<uint8_t, 16> stack = {};
    uint8_t sp;

    uint8_t delayTimer;
    uint8_t soundTimer;
};

