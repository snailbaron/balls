#include "intersection.hpp"

#include <algorithm>

bool intersect(const Point& point, const Circle& circle)
{
    return squareDistance(point, circle.center) <= circle.radius * circle.radius;
}

bool intersect(const Circle& lhs, const Circle& rhs)
{
    return squareDistance(lhs.center, rhs.center) <=
        (lhs.radius + rhs.radius) * (lhs.radius + rhs.radius);
}

bool intersect(const Point& point, const AxisAlignedRect& rect)
{
    return rect.containsPoint(point);
}

bool intersect(const AxisAlignedRect& rect, const Point& point)
{
    return intersect(point, rect);
}

bool intersect(const AxisAlignedRect& lhs, AxisAlignedRect& rhs)
{
    return std::abs(rhs.center.x - lhs.center.x) <= (lhs.width + rhs.width) / 2 &&
        std::abs(rhs.center.y - lhs.center.y) <= (lhs.height + rhs.height) / 2;
}

bool intersect(const Circle& circle, const AxisAlignedRect& rect)
{
    if (intersect(circle.center, rect)) {
        return true;
    }

    float dx = std::min(
        std::abs(circle.center.x - rect.minX()),
        std::abs(circle.center.y - rect.maxX()));
    float dy = std::min(
        std::abs(circle.center.y - rect.minY()),
        std::abs(circle.center.y - rect.maxY()));
    return (dx * dx + dy * dy) <= circle.radius * circle.radius;
}

bool intersect(const Circle& circle, const Line& line)
{
    float circleCenterValue = dot(line.norm(), circle.center - Point{0, 0});
    return std::abs(circleCenterValue - line.value()) <= circle.radius;
}

bool intersect(const Line& line, const Circle& circle)
{
    return intersect(circle, line);
}

bool intersect(const Line& line, const AxisAlignedRect& rect)
{
    auto v1 = Vector{rect.minX(), rect.minY()};
    auto v2 = Vector{rect.minX(), rect.maxY()};
    auto v3 = Vector{rect.maxX(), rect.maxY()};
    auto v4 = Vector{rect.maxX(), rect.minY()};

    float r1 = dot(line.norm(), v1) - line.value();
    float r2 = dot(line.norm(), v2) - line.value();
    float r3 = dot(line.norm(), v3) - line.value();
    float r4 = dot(line.norm(), v4) - line.value();

    return r1 * r3 <= 0 || r2 * r4 <= 0;
}

bool intersect(const AxisAlignedRect& rect, const Line& line)
{
    return intersect(line, rect);
}

std::optional<Point> intersectionPoint(const Line& lhs, const Line& rhs)
{
    float d = det(lhs.norm(), rhs.norm());
    if (d == 0) {
        return std::nullopt;
    }

    auto valueVector = Vector{lhs.value(), rhs.value()};
    float dx = det(valueVector, Vector{lhs.norm().y(), rhs.norm().y()});
    float dy = det(Vector{lhs.norm().x(), rhs.norm().x()}, valueVector);

    return Point{dx / d, dy / d};
}

std::optional<Point> intersectionPoint(const Ray& ray, const Line& line)
{
    auto point = intersectionPoint(ray.line(), line);
    if (point && ray.containsProjection(*point)) {
        return *point;
    }
    return std::nullopt;
}

std::optional<Point> intersectionPoint(const Line& line, const Segment& segment)
{
    auto point = intersectionPoint(line, segment.line());
    if (point && segment.containsProjection(*point)) {
        return *point;
    }
    return std::nullopt;
}

std::optional<Point> intersectionPoint(const Ray& ray, const Segment& segment)
{
    auto p = intersectionPoint(ray.line(), segment.line());
    if (p && ray.containsProjection(*p) && segment.containsProjection(*p)) {
        return *p;
    }
    return std::nullopt;
}