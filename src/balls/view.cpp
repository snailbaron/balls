#include "view.hpp"

#include "config.hpp"
#include "error.hpp"

View::View(World& world)
    : _world(world)
{
    _window.reset(checkSdl(SDL_CreateWindow(
        config.windowTitle.c_str(),
        config.windowWidth,
        config.windowHeight,
        0)));

    _renderer.reset(checkSdl(SDL_CreateRenderer(
        _window.get(),
        nullptr,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)));

    checkSdl(SDL_HideCursor());
}

bool View::processInput() const
{
    for (SDL_Event event; SDL_PollEvent(&event); ) {
        if (event.type == SDL_EVENT_QUIT) {
            return false;
        }

        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            _world.setControl(event.motion.x);
        }
    }

    return true;
}

void View::update(float delta)
{
    (void)delta;
}

void View::render()
{
    _renderer.setDrawColor(30, 30, 30, 255);
    _renderer.clear();

    _renderer.setDrawColor(200, 200, 200, 255);
    _renderer.fillRect(SDL_FRect{
        .x = _world.pad().minX(),
        .y = _world.pad().minY(),
        .w = _world.pad().width,
        .h = _world.pad().height,
    });

    _renderer.setDrawColor(200, 100, 100, 255);
    for (const auto& brick : _world.bricks()) {
        _renderer.fillRect(SDL_FRect{
            .x = brick.minX(),
            .y = brick.minY(),
            .w = brick.width,
            .h = brick.height,
        });
    }

    _renderer.present();
}
