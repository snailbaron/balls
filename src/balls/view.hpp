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

    sdl::Window _window;
    sdl::Renderer _renderer;
};