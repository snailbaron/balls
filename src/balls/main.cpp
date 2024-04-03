#include "config.hpp"
#include "error.hpp"
#include "timer.hpp"
#include "view.hpp"
#include "world.hpp"

#include <SDL.h>

#include <memory>

class SdlInit {
public:
    SdlInit(Uint32 flags)
    {
        checkSdl(SDL_Init(flags));
    }

    ~SdlInit()
    {
        SDL_Quit();
    }
};

int main()
{
    auto sdlInit = SdlInit{SDL_INIT_VIDEO | SDL_INIT_AUDIO};

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