#pragma once

#include "ecs.hpp"
#include "geometry.hpp"

#include <vector>

struct Movement {
    Point position;
    Vector velocity;
};

struct Ball {
    Circle body;
    Vector velocity;
};

class World {
public:
    World();

    void update(float delta);
    void setControl(float padPosition);

    std::span<const AxisAlignedRect> bricks() const;
    const AxisAlignedRect& pad() const;

private:
    std::vector<AxisAlignedRect> _bricks;
    Ball _ball;
    AxisAlignedRect _pad;
};