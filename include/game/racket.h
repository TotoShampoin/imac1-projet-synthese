#pragma once

#include "logic/vec3.h"
#include "IHM/objects.h"
#include "logic/physics.h"

#include <vector>

struct Racket {
    Vec3f position = Vec3f(0,0,0);
    Vec3f scale = Vec3f(0.3, 0.3, 0.3);
    PhysicsAABB hitbox = PhysicsAABB (Vec3f(0, 0, 0), Vec3f(0, 0, 0));
    bool hasBall = true;
    bool isMovingForward = false;
};

Geometry makeRacketMesh();
