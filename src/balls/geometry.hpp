#pragma once

#include <array>
#include <cmath>
#include <ostream>

class Norm;

struct Vector {
    Vector& operator+=(const Vector& other);
    Vector& operator-=(const Vector& other);
    Vector& operator*=(float scalar);
    Vector& operator/=(float scalar);

    [[nodiscard]] Norm normalized() const;
    [[nodiscard]] Vector rotatedCw() const;
    [[nodiscard]] Vector rotatedCcw() const;

    float squareLength() const;
    float length() const;
    void normalize();

    float x = 0.f;
    float y = 0.f;
};

bool operator==(const Vector& lhs, const Vector& rhs);
bool operator!=(const Vector& lhs, const Vector& rhs);

Vector operator-(const Vector& vector);
Vector operator+(Vector lhs, const Vector& rhs);
Vector operator-(Vector lhs, const Vector& rhs);
Vector operator*(Vector vector, float scalar);
Vector operator*(float scalar, Vector vector);
Vector operator/(Vector vector, float scalar);

float dot(const Vector& lhs, const Vector& rhs);
float cross(const Vector& lhs, const Vector& rhs);
bool leftTurn(const Vector& lhs, const Vector& rhs);
bool rightTurn(const Vector& lhs, const Vector& rhs);

class Norm {
public:
    explicit Norm(Vector vector)
        : _vector(vector.normalized())
    { }

    float x() const
    {
        return _vector.x;
    }

    float y() const
    {
        return _vector.y;
    }

    operator const Vector&() const
    {
        return _vector;
    }

    [[nodiscard]] Norm rotatedCw() const;
    [[nodiscard]] Norm rotatedCcw() const;

private:
    Norm(float x, float y)
        : _vector{x, y}
    { }

    Vector _vector;
};

struct Point {
    Point& operator+=(const Vector& vector);
    Point& operator-=(const Vector& vector);

    Vector asVector() const
    {
        return Vector{x, y};
    }

    static const Point origin;

    float x = 0.f;
    float y = 0.f;
};

inline const Point Point::origin {0, 0};

bool operator==(const Point& lhs, const Point& rhs);
bool operator!=(const Point& lhs, const Point& rhs);

Point operator+(Point point, const Vector& vector);
Point operator-(Point point, const Vector& vector);
Vector operator-(const Point& lhs, const Point& rhs);

class Line {
public:
    Line(const Vector& norm, float value)
        : _norm(norm)
        , _value(value)
    { }

    Line(const Point& point, const Vector& direction)
        : _norm(normFromDirection(direction))
        , _value(coordinate(point))
    { }

    float coordinate(const Point& point) const
    {
        return dot(direction(), point - Point::origin);
    }

    Point pointAtCoordinate(float coordinate) const
    {
        return point() + direction() * coordinate;
    }

    const Norm& norm() const
    {
        return _norm;
    }

    float value() const
    {
        return _value;
    }

    Point point() const
    {
        return Point::origin + _norm * _value;
    }

    Norm direction() const
    {
        return directionFromNorm(_norm);
    }

    float signedDistance(const Point& point) const
    {
        return dot(_norm, point - Point::origin);
    }

    void moveTowards(const Point& point, float amount)
    {
        if (dot(_norm, point.asVector()) >= _value) {
            _value += amount;
        } else {
            _value -= amount;
        }
    }

    [[nodiscard]] Line movedTowards(const Point& point, float amount) const
    {
        auto moved = *this;
        moved.moveTowards(point, amount);
        return moved;
    }

private:
    static Norm normFromDirection(const Vector& direction)
    {
        return Norm{direction.rotatedCw()};
    }

    static Norm directionFromNorm(const Norm& norm)
    {
        return norm.rotatedCcw();
    }

    Norm _norm;
    float _value;
};

class Ray {
public:
    Ray(const Point& point, const Vector& direction)
        : _line(point, direction)
        , _start(_line.coordinate(point))
    { }

    const Line& line() const
    {
        return _line;
    }

    bool containsProjection(const Point& point) const
    {
        return _line.coordinate(point) >= _start;
    }

private:
    Line _line;
    float _start = 0.f;
};

class Segment {
public:
    Segment(const Point& firstPoint, const Point& secondPoint)
        : _line(firstPoint, secondPoint - firstPoint)
        , _start(_line.coordinate(firstPoint))
        , _end(_line.coordinate(secondPoint))
    {
        if (_start > _end) {
            std::swap(_start, _end);
        }
    }

    const Line& line() const
    {
        return _line;
    }

    const Norm& norm() const
    {
        return _line.norm();
    }

    Point start() const
    {
        return _line.pointAtCoordinate(_start);
    }

    Point end() const
    {
        return _line.pointAtCoordinate(_end);
    }

    void moveTowards(const Point& point, float amount)
    {
        _line.moveTowards(point, amount);
    }

    [[nodiscard]] Segment movedTowards(const Point& point, float amount) const
    {
        auto moved = *this;
        moved.moveTowards(point, amount);
        return moved;
    }

    bool containsProjection(const Point& point) const
    {
        float pointCoordinate = _line.coordinate(point);
        return pointCoordinate >= _start && pointCoordinate <= _end;
    }

private:
    Line _line;
    float _start = 0.f;
    float _end = 0.f;
};

struct Circle {
    Point center;
    float radius = 0.f;
};

struct AxisAlignedRect {
    std::array<Point, 4> points() const
    {
        return {
            Point{center.x - width / 2, center.y - width / 2},
            Point{center.x - width / 2, center.y + width / 2},
            Point{center.x + width / 2, center.y + width / 2},
            Point{center.x + width / 2, center.y - width / 2},
        };
    }

    std::array<Segment, 4> segments() const
    {
        auto ps = points();
        return {
            Segment{ps[0], ps[1]},
            Segment{ps[1], ps[2]},
            Segment{ps[2], ps[3]},
            Segment{ps[3], ps[0]},
        };
    }

    bool containsPoint(const Point& point) const
    {
        return std::abs(point.x - center.x) <= width / 2 &&
            std::abs(point.y - center.y) <= height / 2;
    }

    float minX() const { return center.x - width / 2; }
    float maxX() const { return center.x + width / 2; }
    float minY() const { return center.y - height / 2; }
    float maxY() const { return center.y + height / 2; }

    Point center;
    float width = 0.f;
    float height = 0.f;
};

std::ostream& operator<<(std::ostream& output, const AxisAlignedRect& rect);

inline float det(const Vector& a, const Vector& b) noexcept
{
    return a.x * b.y - a.y * b.x;
}

float squareDistance(const Point& lhs, const Point& rhs);
Vector mirror(const Vector& vector, const Norm& norm);

Point closestPoint(const Line& line, const Point& point);