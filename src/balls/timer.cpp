#include "timer.hpp"

#include <thread>

FrameTimer::FrameTimer(int fps)
    : _frameDurationSeconds(1.f / fps)
    , _frameDuration(
        std::chrono::duration_cast<Clock::duration>(
            std::chrono::duration<double>(1.0 / fps)))
    , _start(Clock::now())
{ }

int FrameTimer::operator()()
{
    const auto now = Clock::now();
    auto currentFrame = (now - _start) / _frameDuration;
    auto delta = static_cast<int>(currentFrame - _lastFrameIndex);
    _lastFrameIndex = currentFrame;
    return delta;
}

float FrameTimer::delta() const
{
    return _frameDurationSeconds;
}

void FrameTimer::relax() const
{
    auto nextFrameTime = _start + _frameDuration * (_lastFrameIndex + 1);
    std::this_thread::sleep_until(nextFrameTime);
}