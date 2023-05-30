#pragma once

struct Vec2f {
    float x, y;

    Vec2f();
    Vec2f(float x, float y);

    Vec2f& operator=(Vec2f other);
    bool operator==(Vec2f other);
    Vec2f operator-();
    Vec2f operator+(Vec2f other);
    Vec2f operator-(Vec2f other);
    Vec2f operator*(float other);
    Vec2f operator/(float other);
    float operator*(Vec2f other);
    Vec2f& operator+=(Vec2f other);
    Vec2f& operator-=(Vec2f other);
    Vec2f& operator*=(float other);
    Vec2f& operator/=(float other);

    float norm();
    float norm2();
    Vec2f normalize();
};
