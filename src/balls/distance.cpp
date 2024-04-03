#include "distance.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <vector>

float distance(const Point& lhs, const Point& rhs)
{
    return (rhs - lhs).length();
}

float distance(const Point& point, const Circle& circle)
{
    return std::max(0.f, distance(point, circle.center) - circle.radius);
}

float distance(const Circle& circle, const Point& point)
{
    return distance(point, circle);
}

float distance(const Point& point, const Line& line)
{
    return std::abs(dot(line.norm(), point - Point{0, 0}) - line.value());
}

float distance(const Line& line, const Point& point)
{
    return distance(point, line);
}

float distance(const Line& line, const Circle& circle)
{
    float circleCenterValue = dot(line.norm(), circle.center - Point{0, 0});
    float valueDifference = std::abs(circleCenterValue - line.value());
    return std::max(0.f, valueDifference - circle.radius);
}

float distance(const Circle& circle, const Line& line)
{
    return distance(line, circle);
}

float distance(const Line& line, AxisAlignedRect& rect)
{
    float minDistance = std::numeric_limits<float>::infinity();
    for (const Point& point : rect.points()) {
        if (float d = distance(line, point); d < minDistance) {
            minDistance = d;
        }
    }
    return minDistance;
}
