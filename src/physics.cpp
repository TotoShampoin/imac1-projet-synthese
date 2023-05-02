#include "physics.h"
#include <cmath>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

bool floatCollide(float _a0, float _a1, float _b0, float _b1) {
    float a0 = min(_a0, _a1);
    float a1 = max(_a0, _a1);
    float b0 = min(_b0, _b1);
    float b1 = max(_b0, _b1);
    return b0 <= a1 && a0 <= b1;
}
bool circlePointCollide(float cx, float cy, float r, float px, float py) {
    float dist2 = (px - cx) * (py - cy);
    return dist2 <= r * r;
}

PhysicsAABB::PhysicsAABB(Vec3f a, Vec3f b) {
    boundA = a;
    boundB = b;
}
PhysicsSphere::PhysicsSphere(Vec3f pos, float r) {
    position = pos;
    radius = r;
}

Vec3f PhysicsAABB::boundMin() {
    return Vec3f {
        min(boundA.x, boundB.x),
        min(boundA.y, boundB.y),
        min(boundA.z, boundB.z)
    };
}
Vec3f PhysicsAABB::boundMax() {
    return Vec3f {
        max(boundA.x, boundB.x),
        max(boundA.y, boundB.y),
        max(boundA.z, boundB.z)
    };
}

bool PhysicsAABB::collide(PhysicsAABB other) {
    bool collide_x = floatCollide(
        this->boundA.x, this->boundB.x,
        other.boundA.x, other.boundB.x
    );
    bool collide_y = floatCollide(
        this->boundA.y, this->boundB.y,
        other.boundA.y, other.boundB.y
    );
    bool collide_z = floatCollide(
        this->boundA.z, this->boundB.z,
        other.boundA.z, other.boundB.z
    );
    return collide_x && collide_y && collide_z;
}

bool PhysicsSphere::collide(Vec3f point) {
    return (point - position).norm2() <= radius*radius;
}

/* https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection#sphere_vs._aabb */
bool PhysicsSphere::collide(PhysicsAABB box) {
    Vec3f closest = Vec3f {
        max(box.boundMin().x, min(position.x, box.boundMax().x)),
        max(box.boundMin().y, min(position.y, box.boundMax().y)),
        max(box.boundMin().z, min(position.z, box.boundMax().z))
    };

    return (closest - position).norm2() < radius*radius;
}
