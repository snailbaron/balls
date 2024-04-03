#include "geometry.hpp"

Vector& Vector::operator+=(const Vector& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vector& Vector::operator-=(const Vector& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector& Vector::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector& Vector::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

Norm Vector::normalized() const
{
    return Norm{*this};
}

Vector Vector::rotatedCw() const
{
    return Vector{y, -x};
}

Vector Vector::rotatedCcw() const
{
    return Vector{-y, x};
}

float Vector::squareLength() const
{
    return x * x + y * y;
}

float Vector::length() const
{
    return std::sqrt(squareLength());
}

void Vector::normalize()
{
    float sqLength = x * x + y * y;
    if (sqLength > 0) {
        float l = std::sqrt(sqLength);
        x /= l;
        y /= l;
    }
}

bool operator==(const Vector& lhs, const Vector& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Vector& lhs, const Vector& rhs)
{
    return !(lhs == rhs);
}

Vector operator-(const Vector& vector)
{
    return Vector{-vector.x, -vector.y};
}

Vector operator+(Vector lhs, const Vector& rhs)
{
    lhs += rhs;
    return lhs;
}

Vector operator-(Vector lhs, const Vector& rhs)
{
    lhs -= rhs;
    return lhs;
}

Vector operator*(Vector vector, float scalar)
{
    vector *= scalar;
    return vector;
}

Vector operator*(float scalar, Vector vector)
{
    vector *= scalar;
    return vector;
}

Vector operator/(Vector vector, float scalar)
{
    vector /= scalar;
    return vector;
}

float dot(const Vector& lhs, const Vector& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

float cross(const Vector& lhs, const Vector& rhs)
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

bool leftTurn(const Vector& lhs, const Vector& rhs)
{
    return cross(lhs, rhs) > 0;
}

bool rightTurn(const Vector& lhs, const Vector& rhs)
{
    return cross(lhs, rhs) < 0;
}

Norm Norm::rotatedCw() const
{
    return Norm{_vector.y, -_vector.x};
}

Norm Norm::rotatedCcw() const
{
    return Norm{-_vector.y, _vector.x};
}

Point& Point::operator+=(const Vector& vector)
{
    x += vector.x;
    y += vector.y;
    return *this;
}

Point& Point::operator-=(const Vector& vector)
{
    x -= vector.x;
    y -= vector.y;
    return *this;
}

bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Point& lhs, const Point& rhs)
{
    return !(lhs == rhs);
}

Point operator+(Point point, const Vector& vector)
{
    point += vector;
    return point;
}

Point operator-(Point point, const Vector& vector)
{
    point -= vector;
    return point;
}

Vector operator-(const Point& lhs, const Point& rhs)
{
    return Vector{lhs.x - rhs.x, lhs.y - rhs.y};
}

std::ostream& operator<<(std::ostream& output, const AxisAlignedRect& rect)
{
    return output << "[" << rect.minX() << ":" << rect.maxX() <<
        " x " << rect.minY() << ":" << rect.maxY() << "]";
}

float squareDistance(const Point& lhs, const Point& rhs)
{
    return (rhs - lhs).squareLength();
}

Vector mirror(const Vector& vector, const Norm& norm)
{
    float vectorCoordinate = dot(vector, norm);
    return vector - 2 * norm * vectorCoordinate;
}

Point closestPoint(const Line& line, const Point& point)
{
    Norm lineDirection = line.direction();
    float pointCoordinate = dot(point.asVector(), lineDirection);
    return line.point() + lineDirection * pointCoordinate;
}
