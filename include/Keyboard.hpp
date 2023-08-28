#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <SDL.h>

template <typename A, std::size_t B>
class Keyboard {
private:
    std::array<A, B> keyboard { 0 };
public:
    Keyboard();
    void set(std::size_t position);
    A &operator[](std::size_t pos);
    void check_input();
};

template<typename A, std::size_t B>
void Keyboard<A, B>::check_input() {
    SDL_PumpEvents();
    auto keystate = SDL_GetKeyboardState(nullptr);
    uint8_t list[] = {SDL_SCANCODE_0,
                 SDL_SCANCODE_1,
                 SDL_SCANCODE_2,
                 SDL_SCANCODE_3,
                 SDL_SCANCODE_4,
                 SDL_SCANCODE_5,
                 SDL_SCANCODE_6,
                 SDL_SCANCODE_7,
                 SDL_SCANCODE_8,
                 SDL_SCANCODE_9,
                 };
    for (int i = 0; i < 9 ; ++i) {
        keyboard[i] = keystate[list[i]];
    }
}

template<typename A, std::size_t B>
A &Keyboard<A, B>::operator[](std::size_t pos) {
    return keyboard[pos];
}

template<typename A, std::size_t B>
Keyboard<A, B>::Keyboard() = default;

template<typename A, std::size_t B>
void Keyboard<A, B>::set(std::size_t position) {
    std::cout << "Key pressed: " << position << std::endl;
    keyboard[position] = 1;
}
