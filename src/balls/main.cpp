#include "config.hpp"
#include "error.hpp"
#include "sdl.hpp"
#include "timer.hpp"
#include "view.hpp"
#include "world.hpp"

#include <memory>

int main()
{
    auto sdlInit = sdl::Init{SDL_INIT_VIDEO | SDL_INIT_AUDIO};

    auto world = World{};
    auto view = View{world};

    auto timer = FrameTimer{config.fps};
    for (;;) {
        if (!view.processInput()) {
            break;
        }

        int framesPassed = timer();
        if (framesPassed > 0) {
            for (int i = 0; i < framesPassed; i++) {
                world.update(timer.delta());
            }
            view.update(timer.delta() * framesPassed);
            view.render();
        }

        timer.relax();
    }
}