#include "window.hpp"

#include "timer.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>

int main() try
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
} catch (const std::exception& e) {
    try {
        std::cerr << e.what() << "\n";
    } catch (...) {
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}