#pragma once

#include "IHM/window.h"
#include "IHM/objects.h"
#include "logic/vec3.h"
#include "game/ball.h"
#include "game/racket.h"

#include <cmath>
#include <iostream>

static const Vec3f BALL_SHIFT = Vec3f(0, 0, 1);

struct Player {
    Ball ball;
    Racket racket;

    void spawn();
    void launchBall();
    void receiveBall();
    void setPosition(float x, float y);
    void makeAllCollisions(std::vector<Obstacle>& obstacles);

    void update(float delta_time);
};


