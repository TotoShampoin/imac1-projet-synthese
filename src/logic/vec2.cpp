#include "logic/vec2.h"
#include <cmath>

Vec2f::Vec2f() {
    this->x = 0;
    this->y = 0;

}
Vec2f::Vec2f(float x, float y) {
    this->x = x;
    this->y = y;

}

Vec2f& Vec2f::operator=(Vec2f other) {
    this->x = other.x;
    this->y = other.y;

    return (*this);
}
bool Vec2f::operator==(Vec2f other) {
    return 
        this->x == other.x &&
        this->y == other.y;
}

Vec2f Vec2f::operator-() {
    return Vec2f {
        -this->x,
        -this->y,
    };
}
Vec2f Vec2f::operator+(Vec2f other) {
    return Vec2f {
        this->x + other.x,
        this->y + other.y,

    };
}
Vec2f Vec2f::operator-(Vec2f other) {
    return Vec2f {
        this->x - other.x,
        this->y - other.y,

    };
}
Vec2f Vec2f::operator*(float other) {
    return Vec2f {
        this->x * other,
        this->y * other,

    };
}
Vec2f Vec2f::operator/(float other) {
    return Vec2f {
        this->x / other,
        this->y / other,

    };
}
float Vec2f::operator*(Vec2f other) {
    return this->x * other.x + this->y * other.y;
}

Vec2f& Vec2f::operator+=(Vec2f other) {
    this->x += other.x;
    this->y += other.y;

    return *this;
}
Vec2f& Vec2f::operator-=(Vec2f other) {
    this->x -= other.x;
    this->y -= other.y;

    return *this;
}
Vec2f& Vec2f::operator*=(float other) {
    this->x *= other;
    this->y *= other;

    return *this;
}
Vec2f& Vec2f::operator/=(float other) {
    this->x /= other;
    this->y /= other;

    return *this;
}

float Vec2f::norm() {
    return sqrt(norm2());
}
float Vec2f::norm2() {
    return (*this) * (*this);
}
Vec2f Vec2f::normalize() {
    return (*this) / this->norm();
}
