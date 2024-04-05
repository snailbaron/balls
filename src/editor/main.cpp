#include "window.hpp"

#include "timer.hpp"

int main()
{
    static constexpr int fps = 240;

    auto window = EditorWindow{};

    auto timer = FrameTimer{fps};
    for (;;) {
        if (!window.processInput()) {
            break;
        }

        if (int framesPassed = timer(); framesPassed > 0) {
            window.update((float)framesPassed * timer.delta());
            window.present();
        }

        timer.relax();
    }
}