#include "world.hpp"

#include "collision.hpp"
#include "distance.hpp"

#include <iostream>

World::World()
    : _ball{
        .body = Circle{.center = Point{10, 3}, .radius = 1},
        .velocity = Vector{1, 3}
    }
    , _pad{{200, 50}, 100, 50}
{
    _bricks.push_back(AxisAlignedRect{{500, 400}, 100, 50});
    _bricks.push_back(AxisAlignedRect{{300, 100}, 100, 50});
}

void World::update(float delta)
{
    std::cerr << "pad position: " << _pad << "\n";

    (void)delta;
    // this is definitely stupid!
    //float distanceToMove = _ball.velocity.length() * delta;
    //while (distanceToMove > 0) {
    //    auto collisions = std::vector<std::optional<Collision>>{};
    //    for (const auto& brick : _bricks) {
    //        collisions.push_back(collide(_ball.body, _ball.velocity, brick));
    //    }

    //    auto best = bestCollision(_ball.body.center, collisions);
    //    if (!best) {
    //        _ball.body.center += _ball.velocity * delta;
    //        break;
    //    }

    //    distanceToMove -= distance(_ball.body.center, best->position);
    //    _ball.body.center = best->position;
    //    _ball.velocity = best->movement;
    //}
}

void World::setControl(float padPosition)
{
    _pad.center.x = padPosition;
}

std::span<const AxisAlignedRect> World::bricks() const
{
    return _bricks;
}

const AxisAlignedRect& World::pad() const
{
    return _pad;
}
