#include "logic/vec3.h"
#include <cmath>

Vec3f::Vec3f() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}
Vec3f::Vec3f(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3f& Vec3f::operator=(Vec3f other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return (*this);
}
bool Vec3f::operator==(Vec3f other) {
    return 
        this->x == other.x &&
        this->y == other.y &&
        this->z == other.z;
}

Vec3f Vec3f::operator-() {
    return Vec3f {
        -this->x,
        -this->y,
        -this->z,
    };
}
Vec3f Vec3f::operator+(Vec3f other) {
    return Vec3f {
        this->x + other.x,
        this->y + other.y,
        this->z + other.z,
    };
}
Vec3f Vec3f::operator-(Vec3f other) {
    return Vec3f {
        this->x - other.x,
        this->y - other.y,
        this->z - other.z,
    };
}
Vec3f Vec3f::operator*(float other) {
    return Vec3f {
        this->x * other,
        this->y * other,
        this->z * other,
    };
}
Vec3f Vec3f::operator/(float other) {
    return Vec3f {
        this->x / other,
        this->y / other,
        this->z / other,
    };
}
float Vec3f::operator*(Vec3f other) {
    return this->x * other.x + this->y * other.y + this->z * other.z;
}

Vec3f& Vec3f::operator+=(Vec3f other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}
Vec3f& Vec3f::operator-=(Vec3f other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}
Vec3f& Vec3f::operator*=(float other) {
    this->x *= other;
    this->y *= other;
    this->z *= other;
    return *this;
}
Vec3f& Vec3f::operator/=(float other) {
    this->x /= other;
    this->y /= other;
    this->z /= other;
    return *this;
}

float Vec3f::norm() {
    return sqrt(norm2());
}
float Vec3f::norm2() {
    return (*this) * (*this);
}
Vec3f Vec3f::normalize() {
    return (*this) / this->norm();
}
