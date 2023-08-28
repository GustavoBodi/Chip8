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
    void set1(std::size_t position);
    void set0(std::size_t position);
    void print();
    A &flip(std::size_t position);
    A &operator[](std::size_t position);
    void clear();
};

template<typename A, std::size_t B>
void Screen<A, B>::print() {
    for(int i = 0; i < screen.size(); ++i) {
        std::cout << i << " : " << (int)screen[i] << " | ";
        if (i % 10 == 0) {
            std::cout << std::endl;
        }
    }
}

template<typename A, std::size_t B>
std::size_t Screen<A, B>::size() {
    return screen.size();
}

template<typename A, std::size_t B>
void Screen<A, B>::clear() {
    for (size_t i = 0; i < screen.size() ; ++i) {
        screen[i] = 0;
    }
}

template<typename A, std::size_t B>
A &Screen<A, B>::operator[](std::size_t position) {
    return screen[position];
}

template<typename A, std::size_t B>
A &Screen<A, B>::flip(std::size_t position) {
    screen [position] = !screen[position];
    return screen[position];
}

template<typename A, std::size_t B>
void Screen<A, B>::set0(std::size_t position) {
    screen[position] = 0;
}

template<typename A, std::size_t B>
void Screen<A, B>::set1(std::size_t position) {
    screen[position] = 1;
}

template<typename A, std::size_t B>
Screen<A, B>::Screen() = default;
