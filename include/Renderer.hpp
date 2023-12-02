#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <iostream>
#include "Keyboard.hpp"

template<typename A>
class Renderer {
private:
    static constexpr std::size_t multiplier = 20;

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
    A &screen;
    std::string name;
public:
    explicit Renderer(std::string &&name, A &screen);
    void render();
};

template<typename A>
Renderer<A>::Renderer(std::string &&name, A &screen):
        window{SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64 * multiplier,
                                32 * multiplier, 0), SDL_DestroyWindow},
        renderer{SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer},
        screen(screen)
        {}

template<typename A>
void Renderer<A>::render() {
    for (std::size_t i = 0; i < screen.size(); ++i) {
        if (screen[i]) {
            SDL_Surface *surface = SDL_GetWindowSurface(window.get());
            SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
            SDL_Rect rect;
            rect.x = i % 64 * multiplier;
            rect.y = i / 64 * multiplier;
            rect.h = multiplier;
            rect.w = multiplier;
            SDL_RenderDrawRect(renderer.get(), &rect);
            SDL_RenderFillRect(renderer.get(), &rect);
        }
    }
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);
    SDL_RenderPresent(renderer.get());
    SDL_RenderClear(renderer.get());
    SDL_Delay(1);
}


