#pragma once

#include <chrono>

class FrameTimer {
public:
    FrameTimer(int fps);

    int operator()();
    float delta() const;
    void relax() const;

private:
    using Clock = std::chrono::high_resolution_clock;

    float _frameDurationSeconds;
    Clock::duration _frameDuration;
    Clock::time_point _start;
    long long _lastFrameIndex = 0;
};