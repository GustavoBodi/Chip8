#include "../include/Chip8.hpp"
#include <array>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <SDL.h>


Chip8::Chip8(Fonts &fonts) : keyboard{std::make_unique<keyboard_t>(keyboard_t())},
                             display{std::make_unique<display_t>(display_t())} {
    for (std::size_t i = font_start; i < font_start + fonts.size(); ++i) {
        memory[i] = fonts[i - font_start];
    }
    build_table();
}

void Chip8::build_table() {
    std::fill(table.begin(), table.end(), [&]() {op_invalid();});
    // Main table, highest byte
    table[0x0] = [&]() { Chip8::Table0(); };
    table[0x1] = [&]() { Chip8::op_1nnn(); };
    table[0x2] = [&]() { Chip8::op_2nnn(); };
    table[0x3] = [&]() { Chip8::op_3xkk(); };
    table[0x4] = [&]() { Chip8::op_4xkk(); };
    table[0x5] = [&]() { Chip8::op_5xy0(); };
    table[0x6] = [&]() { Chip8::op_6xkk(); };
    table[0x7] = [&]() { Chip8::op_7xkk(); };
    table[0x8] = [&]() { Chip8::Table8(); };
    table[0x9] = [&]() { Chip8::op_9xy0(); };
    table[0xA] = [&]() { Chip8::op_Annn(); };
    table[0xB] = [&]() { Chip8::op_Bnnn(); };
    table[0xC] = [&]() { Chip8::op_Cxkk(); };
    table[0xD] = [&]() { Chip8::op_Dxyn(); };
    table[0xE] = [&]() { Chip8::TableE(); };
    table[0xF] = [&]() { Chip8::TableF(); };

    std::fill(table0.begin(), table0.end(), [&]() {op_invalid();});
    // Table 0, lowest nibble
    table0[0x0] = [&]() { Chip8::op_00E0(); };
    table0[0xE] = [&]() { Chip8::op_00EE(); };

    std::fill(table8.begin(), table8.end(), [&]() {op_invalid();});
    // Table 8, lowest nibble
    table8[0x0] = [&]() { Chip8::op_8xy0(); };
    table8[0x1] = [&]() { Chip8::op_8xy1(); };
    table8[0x2] = [&]() { Chip8::op_8xy2(); };
    table8[0x3] = [&]() { Chip8::op_8xy3(); };
    table8[0x4] = [&]() { Chip8::op_8xy4(); };
    table8[0x5] = [&]() { Chip8::op_8xy5(); };
    table8[0x6] = [&]() { Chip8::op_8xy6(); };
    table8[0x7] = [&]() { Chip8::op_8xy7(); };
    table8[0xE] = [&]() { Chip8::op_8xyE(); };

    std::fill(tableE.begin(), tableE.end(), [&]() {op_invalid();});
    // Table E, lowest nibble
    tableE[0xE] = [&]() { Chip8::op_Ex9E(); };
    tableE[0x1] = [&]() { Chip8::op_ExA1(); };

    std::fill(tableF.begin(), tableF.end(), [&]() {op_invalid();});
    // Table F, lower byte
    tableF[0x07] = [&] () { Chip8::op_Fx07(); };
    tableF[0x0A] = [&] () { Chip8::op_Fx0A(); };
    tableF[0x15] = [&] () { Chip8::op_Fx15(); };
    tableF[0x18] = [&] () { Chip8::op_Fx18(); };
    tableF[0x1E] = [&] () { Chip8::op_Fx1E(); };
    tableF[0x29] = [&] () { Chip8::op_Fx29(); };
    tableF[0x33] = [&] () { Chip8::op_Fx33(); };
    tableF[0x55] = [&] () { Chip8::op_Fx55(); };
    tableF[0x65] = [&] () { Chip8::op_Fx65(); };
}

void Chip8::print_internal_memory() {
    for (uint16_t i = 0; i < memory_size; ++i) {
        if (i % 10 == 0) {
            std::cout << std::endl;
        }
        std::cout << std::setfill('0') << std::setw(3) << std::hex << (0xFFF & i) << " : ";
        std::cout << std::setfill('0') << std::setw(2) << std::hex << (0xFF & memory[i]) << " | ";
    }
    std::cout << std::dec;
}

void Chip8::load_file(std::string &&filename) {
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {
        std::copy(std::istreambuf_iterator<char>{file}, {}, memory.begin() + 512);
    } else {
        std::cout << "Couldn't read file" << std::endl;
        exit(0);
    }
}

void Chip8::fetch() {
    opcode = (static_cast<instruction_t>(memory[pc]) << 8) | static_cast<instruction_t>(memory[pc + 1]);
    pc += 2;
}

Chip8::display_t &Chip8::get_display() {
    return *display;
}

void Chip8::decode() {
    nnn = 0x0FFF & opcode;
    n = 0x000F & opcode;
    x = (0x0F00 & opcode) >> 8;
    y = (0x00F0 & opcode) >> 4;
    kk = 0x00FF & opcode;
}

void Chip8::execute() {
    Table();
    if (dt > 0) {
        --dt;
    }
    if (st > 0) {
        --st;
    }
}

void Chip8::op_00E0() {
    display->clear();
}

void Chip8::op_00EE() {
    pc = stack[sp];
    sp -= 1;
}

void Chip8::op_1nnn() {
    pc = nnn;
}

void Chip8::op_2nnn() {
    sp += 1;
    stack[sp] = pc;
    pc = nnn;
}

void Chip8::op_3xkk() {
    if (V[x] == kk) {
        pc += 2;
    }
}

void Chip8::op_4xkk() {
    if (V[x] != kk) {
        pc += 2;
    }
}

void Chip8::op_5xy0() {
    if (V[x] == V[y]) {
        pc += 2;
    }
}

void Chip8::op_6xkk() {
    V[x] = kk;
}

void Chip8::op_7xkk() {
    V[x] += kk;
}

void Chip8::op_8xy0() {
    V[x] = V[y];
}

void Chip8::op_8xy1() {
    V[x] |= V[y];
    V[0xF] = 0;
}

void Chip8::op_8xy2() {
    V[x] &= V[y];
    V[0xF] = 0;
}

void Chip8::op_8xy3() {
    V[x] ^= V[y];
    V[0xF] = 0;
}

void Chip8::op_8xy4() {
    uint16_t result = static_cast<uint16_t>(V[x]) + static_cast<uint16_t>(V[y]);
    V[x] = static_cast<uint8_t>(0x00FF & result);
    if (result > 255) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }
}

void Chip8::op_8xy5() {
    auto old = V[x];
    V[x] -= V[y];
    if (old > V[y]) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }
}

void Chip8::op_8xy6() {
    V[x] = V[y];
    auto old = V[x];
    V[x] >>= 1;
    if (old & 0x1) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }
}

void Chip8::op_8xy7() {
    V[x] = V[y] - V[x];
    if (V[y] > V[x]) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }
}

void Chip8::op_8xyE() {
    V[x] = V[y];
    auto old = V[x];
    V[x] <<= 1;
    if ((old & 0x80) >> 7) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }
}

void Chip8::op_9xy0() {
    if (V[x] != V[y]) {
        pc += 2;
    }
}

void Chip8::op_Annn() {
    I = nnn;
}

void Chip8::op_Bnnn() {
    pc = nnn + V[0];
}

void Chip8::op_Cxkk() {
    V[x] = gen_random_byte() && kk;
}

uint8_t Chip8::gen_random_byte() {
    return static_cast<uint8_t>(dist(rng));
}

void Chip8::op_Ex9E() {
    if ((*keyboard)[V[x]]) {
        pc += 2;
    }
}

void Chip8::op_ExA1() {
    if (!(*keyboard)[V[x]]) {
        pc += 2;
    }
}

void Chip8::op_Fx07() {
    V[x] = dt;
}

void Chip8::op_Fx0A() {
    std::cout << "Fx0A" << std::endl;
    int value = check_input();
    if (value == -1) {
        pc -= 2;
        return;
    } else {
        V[x] = value;
    }
}

void Chip8::op_Fx15() {
    dt = V[x];
}

void Chip8::op_Fx18() {
    st = V[x];
}

void Chip8::op_Fx1E() {
    I += V[x];
}

void Chip8::op_Fx33() {
    uint8_t hundreds = V[x] / 100;
    uint8_t tens = (V[x] % 100) / 10;
    uint8_t ones = V[x] % 10;
    memory[I] = hundreds;
    memory[I + 1] = tens;
    memory[I + 2] = ones;
}

void Chip8::op_Fx55() {
    for (std::size_t i = I; i < I + x + 1; ++i) {
        memory[i] = V[i - I];
    }
    I += 1;
}

void Chip8::op_Fx65() {
    for (std::size_t i = I; i < I + x + 1; ++i) {
        V[i - I] = memory[i];
    }
    I += 1;
}

void Chip8::op_Dxyn() {
    V[0xF] = 0;
    for (std::size_t byte = 0; byte < n; ++byte) {
        uint8_t coord_y = V[y] + byte % height;
        for (std::size_t j = 0; j < 8; ++j) {
            uint8_t coord_x = V[x] + j % width;
            uint8_t color = (memory[I + byte] >> (7 - j)) & 1;
            V[0xF] |= color & (*display)[coord_x + coord_y * width];
            (*display)[coord_x + coord_y * width] ^= color;
        }
    }
}

void Chip8::op_Fx29() {
    I = font_start + V[x] * 8;
}

void Chip8::Table0() {
    table0[0x000F & opcode]();
}

void Chip8::Table8() {
    table8[0x000F & opcode]();
}

void Chip8::TableE() {
    tableE[0x000F & opcode]();
}

void Chip8::TableF() {
    tableF[0x00FF & opcode]();
}

void Chip8::Table() {
    table[(0xF000 & opcode) >> 12]();
}

void Chip8::op_invalid() const {
    std::cout << "Invalid operation: " << opcode << std::endl;
}

int Chip8::check_input() {
    keyboard->check_input();
    for (int i = 0; i < 9; ++i) {
        if ((*keyboard)[i]) {
            return i;
        }
    }
    return -1;
}
