#pragma once

#include <SDL.h>

#include <memory>

namespace sdl {

class Renderer {
public:
    explicit Renderer(SDL_Renderer* ptr = nullptr);

    void reset(SDL_Renderer* ptr);

    void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void clear();
    void present();
    void fillRect(const SDL_FRect& rect);

private:
    SDL_Renderer* ptr()
    {
        return _ptr.get();
    }

    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> _ptr;
};

} // namespace sdl