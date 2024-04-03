#include "collision.hpp"

#include "distance.hpp"
#include "intersection.hpp"

#include <vector>

std::optional<Collision> collide(const Point& point, const Vector& movement, const Line& line)
{
    auto intersection = intersectionPoint(Line{point, movement}, line);
    if (!intersection) {
        return std::nullopt;
    }

    return Collision{
        .position = *intersection,
        .movement = mirror(movement, line.norm()),
    };
}

std::optional<Collision> collide(const Point& point, const Vector& movement, const Circle& circle)
{
    auto movementLine = Line{point, movement};
    Point closestToCenter = closestPoint(movementLine, circle.center);

    float sq = squareDistance(closestToCenter, circle.center);
    if (sq >= circle.radius * circle.radius) {
        return std::nullopt;
    }

    float d = std::sqrt(sq);
    float missingDistance = std::sqrt(circle.radius * circle.radius - d * d);
    Norm shiftDirection = (point - closestToCenter).normalized();
    Point collisionPoint = closestToCenter + shiftDirection * missingDistance;

    return Collision{
        .position = collisionPoint,
        .movement = mirror(movement, Norm{collisionPoint - circle.center}),
    };
}

std::optional<Collision> collide(const Point& point, const Vector& movement, const Segment& segment)
{
    auto movementRay = Ray{point, movement};
    if (auto p = intersectionPoint(movementRay, segment)) {
        return Collision{
            .position = *p,
            .movement = mirror(movement, segment.norm()),
        };
    }
    return std::nullopt;
}

std::optional<Collision> collide(const Circle& circle, const Vector& movement, const Point& point)
{
    return collide(
        circle.center,
        movement,
        Circle{.center = point, .radius = circle.radius});
}

std::optional<Collision> collide(
    const Circle& circle, const Vector& movement, const Line& line)
{
    Line shiftedLine = line.movedTowards(circle.center, circle.radius);
    return collide(circle.center, movement, shiftedLine);
}

std::optional<Collision> collide(const Circle& circle, const Vector& movement, const Segment& segment)
{
    Segment movedSegment = segment.movedTowards(circle.center, circle.radius);
    auto firstCircle = Circle{segment.start(), circle.radius};
    auto secondCircle = Circle{segment.end(), circle.radius};

    auto movementRay = Ray{circle.center, movement};
    return bestCollision(
        circle.center,
        std::array{
            collide(circle.center, movement, movedSegment),
            collide(circle.center, movement, firstCircle),
            collide(circle.center, movement, secondCircle),
        });
}

std::optional<Collision> collide(
    const Circle& circle, const Vector& movement, const AxisAlignedRect& rect)
{
    // ok, this is pretty stupid
    auto collisions = std::vector<std::optional<Collision>>{};
    for (const auto& segment : rect.segments()) {
        collisions.push_back(collide(circle, movement, segment));
    }
    return bestCollision(circle.center, collisions);
}