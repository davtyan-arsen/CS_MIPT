#define CATCH_CONFIG_MAIN

#include <iostream>
#include "vector2d.h"

#include "../catch.hpp"

TEST_CASE("Init with no args")
{
    CHECK_NOTHROW(Vector2d());
}

TEST_CASE("Init with args")
{
    CHECK_NOTHROW(Vector2d(3.2343, -4.33321));
}

TEST_CASE("operator==")
{
    Vector2d v1 = {3, 4};
    Vector2d v2 = {3.0, 4.0};

    CHECK(v1 == v2);
}

TEST_CASE("operator!=")
{
    Vector2d v1 = {3, 4};
    Vector2d v2 = {3.0002, 3.9999998};

    CHECK(v1 != v2);
}

TEST_CASE("operator+")
{
    Vector2d v1 = {2.5, 3.5};
    Vector2d v2 = {-0.5, 0.6};
    Vector2d v3 = {2, 4.1};

    CHECK(v1 + v2 == v3);
}

TEST_CASE("operator+=")
{
    Vector2d v1 = {2.5, 3.5};
    Vector2d v2 = {-0.5, 0.6};
    Vector2d v3 = {2, 4.1};

    v1 += v2;

    CHECK(v1 == v3);
}

TEST_CASE("operator-")
{
    Vector2d v1 = {2.5, 3.5};
    Vector2d v2 = {-0.57, 0.6};
    Vector2d v3 = {3.07, 2.9};

    CHECK(v1 - v2 == v3);
}

TEST_CASE("operator-=")
{
    Vector2d v1 = {2.5, 3.5};
    Vector2d v2 = {-0.57, 0.6};
    Vector2d v3 = {3.07, 2.9};

    v1 -= v2;

    CHECK(v1 == v3);
}

TEST_CASE("operator* // vector x float")
{
    Vector2d v1 = {0.44, 0};
    float a = 2;
    Vector2d v3 = {0.88, 0};

    CHECK(v1 * a == v3);
}

TEST_CASE("operator* // scalar multiplication")
{
    Vector2d v1 = {3, 4};
    Vector2d v2 = {-4, 5};

    CHECK(v1 * v2 == 8);
}

TEST_CASE("operator* // float x vector")
{
    float a = 0.1;
    Vector2d v1 = {3.0, 4.0};
    Vector2d v3 = {0.3, 0.4};

    CHECK(a * v1 == v3);
}

TEST_CASE("operator^")
{
    Vector2d v1 = {3, 4};
    Vector2d v2 = {1.5, 3};

    float a = v1 ^ v2;

    CHECK(a == 3);
}

TEST_CASE("norm()")
{
    Vector2d v1 = {3, 4};
    Vector2d v2 = {0.6, 0.8};
    v1.norm();

    CHECK(v1 == v2);
}

TEST_CASE("perpendicular()")
{
    Vector2d v1 = {3, 4};
    Vector2d v2 = {-4, 3};
    Vector2d v3 = v1.perpendicular();

    CHECK(v3 == v2);
}

TEST_CASE("operator- // unary")
{
    Vector2d v1 = {3, 4};
    Vector2d v2 = {-3, -4};
    Vector2d v3 = -v1;

    CHECK(v3 == v2);
}

TEST_CASE("rotate")
{
    Vector2d v1 = {1, 0};
    Vector2d v2 = {-1, 0};
    v1.rotate(M_PI);
    CHECK(v1.getX() - v2.getX() < 1e-07);
    CHECK(v1.getY() - v2.getY() < 1e-07);
}

TEST_CASE("getRotated")
{
    Vector2d v1 = {1, 0};
    Vector2d v2 = {0, 1};
    Vector2d v3 = v1.getRotated(M_PI / 2);

    CHECK(v3.getX() - v2.getX() < 1e-07);
    CHECK(v3.getY() - v2.getY() < 1e-07);
}

TEST_CASE("getSquareLength()")
{
    Vector2d v = {3, 4};
    CHECK(v.getSquareLength() == 25);
}

TEST_CASE("getLength()")
{
    Vector2d v = {3, 4};
    CHECK(v.getLength() == 5);
}

TEST_CASE("Console Input and Output")
{
    std::cout << "Input/Output test :: Enter a vector" << std::endl;
    Vector2d v;
    CHECK_NOTHROW(std::cin >> v);
    CHECK_NOTHROW(std::cout << v);
}
