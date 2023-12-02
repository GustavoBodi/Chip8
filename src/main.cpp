#include <iostream>
#include <array>
#include "../include/Chip8.hpp"
#include "../include/Renderer.hpp"
#include "../include/Fonts.hpp"
#include <SDL2/SDL.h>

int main(int argc, char **argv) {
    if (argc != 2) {
      std::cout << "The correct program usage is: ./chip8 <name_of_rom>" << std::endl;
    }
    std::string rom_name = argv[1];
    Fonts font {};
    Chip8 chip = Chip8(font);
    chip.load_file(rom_name);
    Renderer renderer ("My screen", chip.get_display());

    while(true) {
        try {
            if (!chip.check_input()) {
              break;
            }

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
