#pragma once

struct Vec3f {
    float x, y, z;

    Vec3f(float x, float y, float z);

    Vec3f& operator=(Vec3f other);
    bool operator==(Vec3f other);
    Vec3f operator-();
    Vec3f operator+(Vec3f other);
    Vec3f operator-(Vec3f other);
    Vec3f operator*(float other);
    Vec3f operator/(float other);
    float operator*(Vec3f other);
    Vec3f& operator+=(Vec3f other);
    Vec3f& operator-=(Vec3f other);
    Vec3f& operator*=(float other);
    Vec3f& operator/=(float other);

    float norm();
    float norm2();
    Vec3f normalize();
};
