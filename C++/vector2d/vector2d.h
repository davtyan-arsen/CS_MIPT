#pragma once

class Vector2d
{
public:
    Vector2d();
    Vector2d(float, float);
    ~Vector2d();
    float getX() const;
    float getY() const;
    void setX(float);
    void setY(float);
    bool operator==(const Vector2d&) const;
    bool operator!=(const Vector2d&) const;
    Vector2d operator+(const Vector2d&) const;
    Vector2d& operator+=(const Vector2d&);
    Vector2d operator-(const Vector2d&) const;
    Vector2d& operator-=(const Vector2d&);
    Vector2d operator*(const float) const;
    float operator*(const Vector2d&) const;
    float operator^(const Vector2d&) const;
    Vector2d& norm();
    Vector2d perpendicular() const;
    Vector2d operator-() const;
    Vector2d& rotate(const float);
    Vector2d getRotated(const float) const;
    float getLength() const;
    float getSquareLength() const;

private:
    float x;
    float y;
};

Vector2d operator*(float, const Vector2d&);
std::ostream& operator<<(std::ostream&, const Vector2d&);
std::istream& operator>>(std::istream&, Vector2d&);
