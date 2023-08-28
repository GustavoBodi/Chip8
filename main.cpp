#include <iostream>
#include <array>
#include "Chip8.hpp"
#include "Renderer.hpp"
#include "Fonts.hpp"
#include <SDL2/SDL.h>

int main() {
    Fonts font {};
    Chip8 chip = Chip8(font);
    chip.load_file("../test.ch8");
    chip.print_internal_memory();
    Renderer renderer ("My screen", chip.get_display());

    while(true) {
        try {
            chip.check_input();

            chip.fetch();

            chip.decode();

            chip.execute();

            renderer.render();

        } catch ( std::exception &e ) {
            std::cout << e.what() << std::endl;
            break;
        }
    }
    std::cout << "End of the emulation" << std::endl;
}
