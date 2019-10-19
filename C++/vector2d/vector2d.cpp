#include <iostream>
#include <cmath>
#include "vector2d.h"

Vector2d::Vector2d()
{
    this->x = 0;
    this->y = 0;
}

Vector2d::Vector2d(float a, float b)
{
    this->x = a;
    this->y = b;
}

Vector2d::~Vector2d() {};

float Vector2d::getX() const
{
    return this->x;
}

float Vector2d::getY() const
{
    return this->y;
}

void Vector2d::setX(float a)
{
    this->x = a;
}

void Vector2d::setY(float b)
{
    this->y = b;
}

bool Vector2d::operator==(const Vector2d& v2) const
{
    return (this->getX() == v2.getX()) && (this->getY() == v2.getY());
}

bool Vector2d::operator!=(const Vector2d& v2) const
{
    return (this->getX() != v2.getX()) || (this->getY() != v2.getY());
}

Vector2d Vector2d::operator+(const Vector2d& v2) const
{
    return Vector2d(this->getX() + v2.getX(), this->getY() + v2.getY());
}

Vector2d& Vector2d::operator+=(const Vector2d& v2)
{
    this->setX(this->getX() + v2.getX());
    this->setY(this->getY() + v2.getY());
    return *this;
}

Vector2d Vector2d::operator-(const Vector2d& v2) const
{
    return Vector2d(this->getX() - v2.getX(), this->getY() - v2.getY());
}

Vector2d& Vector2d::operator-=(const Vector2d& v2)
{
    this->setX(this->getX() - v2.getX());
    this->setY(this->getY() - v2.getY());
    return *this;
}

Vector2d Vector2d::operator*(const float a) const
{
    return Vector2d(this->getX() * a, this->getY() * a);
}

float Vector2d::operator*(const Vector2d& v2) const
{
    return (this->getX() * v2.getX() + this->getY() * v2.getY());
}

Vector2d operator*(float a, const Vector2d& v)
{
    return Vector2d(a * v.getX(), a * v.getY());
}

float Vector2d::operator^(const Vector2d& v2) const
{
    return (this->getX() * v2.getY() - this->getY() * v2.getX());
}

Vector2d& Vector2d::norm()
{
    float len = this->getLength();
    this->setX(this->getX() / len);
    this->setY(this->getY() / len);
    return *this;
}

Vector2d Vector2d::perpendicular() const
{
    return Vector2d{-(this->getY()), this->getX()};
}

Vector2d Vector2d::operator-() const
{
    return Vector2d{-(this->getX()), -(this->getY())};
}

Vector2d& Vector2d::rotate(const float angle)
{
    this->setX(cos(angle) * this->getX() - sin(angle) * this->getY());
    this->setY(sin(angle) * this->getX() + cos(angle) * this->getY());
    return *this;
}

Vector2d Vector2d::getRotated(const float angle) const
{
    Vector2d v;
    v.setX(cos(angle) * this->getX() - sin(angle) * this->getY());
    v.setY(sin(angle) * this->getX() + cos(angle) * this->getY());
    return v;
}

float Vector2d::getSquareLength() const
{
    return (this->getX() * this->getX() + this->getY() * this->getY());
}

float Vector2d::getLength() const
{
    return sqrt(this->getX() * this->getX() + this->getY() * this->getY());
}

std::ostream& operator<<(std::ostream& os, const Vector2d& v)
{
    os << "(" << v.getX() << ", "<< v.getY() << ")";
    return os;
}

std::istream& operator>>(std::istream& is, Vector2d& v)
{
    float a, b;
    is >> a >> b;
    v.setX(a);
    v.setY(b);
    return is;
}
