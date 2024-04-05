#pragma once

#include "geometry.hpp"

#include <optional>

bool intersect(const Point& point, const Circle& circle);
bool intersect(const Circle& lhs, const Circle& rhs);
bool intersect(const Point& point, const AxisAlignedRect& rect);
bool intersect(const AxisAlignedRect& rect, const Point& point);
bool intersect(const AxisAlignedRect& lhs, AxisAlignedRect& rhs);
bool intersect(const Circle& circle, const AxisAlignedRect& rect);
bool intersect(const Circle& circle, const Line& line);
bool intersect(const Line& line, const Circle& circle);
bool intersect(const Line& line, const AxisAlignedRect& rect);
bool intersect(const AxisAlignedRect& rect, const Line& line);

std::optional<Point> intersectionPoint(const Line& lhs, const Line& rhs);
std::optional<Point> intersectionPoint(const Line& line, const Segment& segment);
std::optional<Point> intersectionPoint(const Ray& ray, const Line& line);
std::optional<Point> intersectionPoint(const Ray& ray, const Segment& segment);

std::optional<Segment> intersectionSegment(const Line& line, const AxisAlignedRect& rect);