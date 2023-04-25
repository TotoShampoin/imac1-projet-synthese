#include "physics.h"

bool float_collide(float a0, float a1, float b0, float b1) {
    return b0 <= a1 && a0 <= b1;
}


PhysicsAABB::PhysicsAABB(Vec3f a, Vec3f b) {
    boundA = a;
    boundB = b;
}
PhysicsSphere::PhysicsSphere(Vec3f pos, float r) {
    position = pos;
    radius = r;
}

bool PhysicsAABB::collide(PhysicsAABB other) {
    bool collide_x = float_collide(
        this->boundA.x, this->boundB.x,
        other.boundA.x, other.boundB.x
    );
    bool collide_y = float_collide(
        this->boundA.y, this->boundB.y,
        other.boundA.y, other.boundB.y
    );
    bool collide_z = float_collide(
        this->boundA.z, this->boundB.z,
        other.boundA.z, other.boundB.z
    );
    return collide_x && collide_y && collide_z;
}

bool PhysicsSphere::collide(Vec3f point) {
    return (point - position).norm2() <= radius*radius;
}
bool PhysicsSphere::collide(PhysicsAABB other, Vec3f& normal) {
    PhysicsAABB bound_of_sphere = PhysicsAABB(
        position - Vec3f(radius/2,radius/2,radius/2),
        position + Vec3f(radius/2,radius/2,radius/2)
    );
    if(!bound_of_sphere.collide(other)) return false;
    if(
        collide(other.boundA) ||
        collide(other.boundB) ||
        collide(Vec3f(other.boundA.x, other.boundA.y, other.boundB.z)) ||
        collide(Vec3f(other.boundA.x, other.boundB.y, other.boundB.z)) ||
        collide(Vec3f(other.boundA.x, other.boundB.y, other.boundA.z)) ||
        collide(Vec3f(other.boundA.x, other.boundB.y, other.boundA.z)) ||
        collide(Vec3f(other.boundB.x, other.boundA.y, other.boundB.z)) ||
        collide(Vec3f(other.boundB.x, other.boundB.y, other.boundB.z)) ||
        collide(Vec3f(other.boundB.x, other.boundB.y, other.boundA.z)) ||
        collide(Vec3f(other.boundB.x, other.boundB.y, other.boundA.z))
    ) return true;
    return false;
}
