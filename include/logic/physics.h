#pragma once
#include "logic/vec3.h"

struct PhysicsAABB {
    Vec3f boundA = Vec3f(0,0,0);
    Vec3f boundB = Vec3f(0,0,0);

    PhysicsAABB(Vec3f a, Vec3f b);

    Vec3f boundMin();
    Vec3f boundMax();
    Vec3f middle();
    Vec3f size();

    bool collide(PhysicsAABB other);
};

struct PhysicsSphere {
    Vec3f position = Vec3f(0,0,0);
    float radius = 0;

    PhysicsSphere(Vec3f pos, float r);

    bool collide(Vec3f point);
    bool collide(PhysicsAABB other);
    bool collide(PhysicsAABB other, Vec3f& normal);
};

typedef PhysicsAABB Obstacle;


bool floatCollide(float a0, float a1, float b0, float b1);
