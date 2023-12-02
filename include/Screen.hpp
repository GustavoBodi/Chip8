#pragma once
#include <cstdint>
#include <array>

template <typename A, std::size_t B>
class Screen {
private:
    std::array<A, B> screen { 0 };
public:
    Screen();
    std::size_t size();
    A &operator[](std::size_t position);
    void clear();
};

template<typename A, std::size_t B>
std::size_t Screen<A, B>::size() {
    return screen.size();
}

template<typename A, std::size_t B>
void Screen<A, B>::clear() {
    for (std::size_t i = 0; i < screen.size() ; ++i) {
        screen[i] = 0;
    }
}

template<typename A, std::size_t B>
A &Screen<A, B>::operator[](std::size_t position) {
    return screen[position];
}

template<typename A, std::size_t B>
Screen<A, B>::Screen() = default;
