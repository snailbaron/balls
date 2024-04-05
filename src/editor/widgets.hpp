#pragma once

#include "geometry.hpp"
#include "sdl.hpp"

#include <memory>
#include <functional>
#include <vector>

class Widget {
public:
    ~Widget() = default;
};

namespace {

}

class UI {
private:
    std::vector<std::unique_ptr<Widget>> _widgets;
};

class Button {
public:
    Button(
        sdl::Renderer& renderer,
        const Point& center,
        std::string text,
        std::function<void()> action)
        : _rect{
            .x = center.x - 50.f,
            .y = center.y - 10.f,
            .w = 100.f,
            .h = 20.f,
        }
        , _action{std::move(action)}
    {
        (void)renderer;
    }

    void act() const
    {
        _action();
    }

    void render(sdl::Renderer& renderer) const
    {
        renderer.fillRect(_rect);
    }

private:
    SDL_FRect _rect;
    std::function<void()> _action;
};