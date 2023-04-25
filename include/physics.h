#include "vec3.h"

struct PhysicsAABB {
    Vec3f boundA = Vec3f(0,0,0);
    Vec3f boundB = Vec3f(0,0,0);

    PhysicsAABB(Vec3f a, Vec3f b);

    bool collide(PhysicsAABB other);
};

struct PhysicsSphere {
    Vec3f position = Vec3f(0,0,0);
    float radius = 0;

    PhysicsSphere(Vec3f pos, float r);

    bool collide(Vec3f point);
    bool collide(PhysicsAABB other, Vec3f& normal);
};
