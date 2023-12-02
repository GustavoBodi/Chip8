#pragma once
#include <cstdint>
#include <string>
#include <array>
#include "Fonts.hpp"
#include "Keyboard.hpp"
#include "Screen.hpp"
#include <memory>
#include <random>
#include <functional>


class Chip8 {
private:
    // All programs start at address 0x200;
    static constexpr int16_t location_start = 0x200;
    // The entire memory has 4096 bytes
    static constexpr std::size_t memory_size = 4096;
    // General purpose registers quantity
    static constexpr std::size_t gr_amount = 16;
    // The size of the stack
    static constexpr std::size_t stack_size = 16;
    // This is the amount of keyboard keys
    static constexpr std::size_t keyboard_size = 16;
    // This is the start in memory address of the fonts
    static constexpr std::size_t font_start = 0x50;
    // Chip8 internal screen width
    static constexpr std::size_t width = 64;
    // Chip8 internal screen height
    static constexpr std::size_t height = 32;
    // Display size is 64 x 32
    static constexpr std::size_t display_pixels = width * height;
    // Represents the instruction type
    using instruction_t = uint16_t;
    // Types that represent the internal keyboard positions
    using keyboard_num_t = uint8_t;
    // The Keyboard Type from template parameters itself
    using keyboard_t = Keyboard<keyboard_num_t, keyboard_size>;
public:
    // Represents the screen type of representation
    using display_t = Screen<uint8_t, display_pixels>;

private:
    std::string name;
    // Memory has 4096 bytes
    std::array<uint8_t, memory_size> memory { 0 };
    // General Purpose registers
    uint8_t V[gr_amount] { 0 };
    // Store Memory Addresses
    uint16_t I { 0 };
    // Delay Timer
    uint8_t dt { 0 };
    // Sound Timer
    uint8_t st { 0 };
    // Program Counter
    uint16_t pc { location_start };
    // Stack Pointer
    uint8_t sp { 0 };
    uint16_t stack[stack_size] { 0 };
    // The keyboard has 16 possible buttons
    std::unique_ptr<keyboard_t> keyboard;
    // Display 64 x 32
    std::unique_ptr<display_t> display;
    // The opcode full instruction
    instruction_t opcode {0};

    // This is the decoded instruction parts
    uint8_t x;
    uint8_t y;
    uint8_t kk;
    uint8_t n;
    uint16_t nnn;

    // Distribution for random number generation
    std::uniform_int_distribution<> dist {0, 255};
    // Random Seed
    std::mt19937 rng {std::random_device{}()};

public:
    explicit Chip8(Fonts &fonts);
    display_t &get_display();
    void print_internal_memory();
    void load_file(std::string &filename);
    void fetch();
    void decode();
    void execute();
    int check_input();

private:
    void build_table();
    uint8_t gen_random_byte();

    using Chip8Table = std::array<std::function<void()>, 0xFF>;

    Chip8Table table;
    Chip8Table table0;
    Chip8Table table8;
    Chip8Table tableE;
    Chip8Table tableF;

    void Table();
    void Table0();
    void Table8();
    void TableE();
    void TableF();

    // Instruction List
    void op_invalid() const;
    void op_00E0();
    void op_00EE();
    void op_1nnn();
    void op_2nnn();
    void op_3xkk();
    void op_4xkk();
    void op_5xy0();
    void op_6xkk();
    void op_7xkk();
    void op_8xy0();
    void op_8xy1();
    void op_8xy2();
    void op_8xy3();
    void op_8xy4();
    void op_8xy5();
    void op_8xy6();
    void op_8xy7();
    void op_8xyE();
    void op_9xy0();
    void op_Annn();
    void op_Bnnn();
    void op_Cxkk();
    void op_Dxyn();
    void op_Ex9E();
    void op_ExA1();
    void op_Fx07();
    void op_Fx0A();
    void op_Fx15();
    void op_Fx18();
    void op_Fx1E();
    void op_Fx29();
    void op_Fx33();
    void op_Fx55();
    void op_Fx65();
};
