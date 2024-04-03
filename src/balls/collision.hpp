#pragma once

#include "geometry.hpp"

#include <limits>
#include <optional>
#include <ranges>

struct Collision {
    Point position;
    Vector movement;
};

std::optional<Collision> bestCollision(
    const Point& start, std::ranges::range auto collisions)
{
    auto bestCollision = std::optional<Collision>{};
    auto bestDistance = std::numeric_limits<float>::infinity();

    for (const auto& collision : collisions) {
        if (!collision) {
            continue;
        }

        if (float d = distance(start, collision->position); d < bestDistance) {
            bestCollision = collision;
            bestDistance = d;
        }
    }

    return bestCollision;
}

std::optional<Collision> collide(
    const Point& point, const Vector& movement, const Line& line);
std::optional<Collision> collide(
    const Point& point, const Vector& movement, const Circle& circle);
std::optional<Collision> collide(
    const Point& point, const Vector& movement, const Segment& segment);

std::optional<Collision> collide(
    const Circle& circle, const Vector& movement, const Point& point);
std::optional<Collision> collide(
    const Circle& circle, const Vector& movement, const Line& line);
std::optional<Collision> collide(
    const Circle& circle, const Vector& movement, const Segment& segment);

std::optional<Collision> collide(
    const Circle& circle, const Vector& movement, const AxisAlignedRect& rect);
