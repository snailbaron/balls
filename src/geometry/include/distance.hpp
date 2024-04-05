#pragma once

#include "geometry.hpp"

float distance(const Point& lhs, const Point& rhs);
float distance(const Point& point, const Circle& circle);
float distance(const Circle& circle, const Point& point);
float distance(const Point& point, const Line& line);
float distance(const Line& line, const Point& point);
float distance(const Line& line, const Circle& circle);
float distance(const Circle& circle, const Line& line);
float distance(const Line& line, AxisAlignedRect& rect);