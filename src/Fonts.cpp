#include "../include/Fonts.hpp"


std::size_t Fonts::size() {
    return font_size;
}

uint8_t Fonts::operator[](uint16_t op) {
    return fonts[op];
}
