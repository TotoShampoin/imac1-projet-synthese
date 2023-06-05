#pragma once

#include "logic/vec3.h"
#include "IHM/objects.h"
#include "logic/physics.h"

#include <vector>

static const float RACKET_SPEED = 2.;

struct Racket {
    Vec3f position = Vec3f(0,0,0);
    Vec3f scale = Vec3f(0.3, 0.3, 0.3);
    PhysicsAABB hitbox = PhysicsAABB (Vec3f(0, 0, 0), Vec3f(0, 0, 0));
    bool hasBall = true;
    bool isMovingForward = false;
    bool canCatchBall = false;
    
    void updateHitbox();
    void setPosition(Vec3f);
    void setPosition(Vec2f);
    void move(float delta_time);

    void update(float delta_time);
};

Geometry makeRacketMesh();
