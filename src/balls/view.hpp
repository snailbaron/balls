#pragma once

#include "world.hpp"

#include "sdl.hpp"

#include <memory>

class View {
public:
    View(World& world);

    bool processInput() const;
    void update(float delta);
    void render();

private:
    World& _world;

    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> _window{
        nullptr, SDL_DestroyWindow};
    sdl::Renderer _renderer;
};