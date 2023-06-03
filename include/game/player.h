#pragma once

#include "IHM/window.h"
#include "IHM/objects.h"
#include "logic/vec3.h"
#include "game/ball.h"
#include "game/racket.h"

#include <cmath>
#include <iostream>

static const int SPAWN_Z = 2;
static const int INIT_LIVES = 5;
static const double RECOVER_TIME_START = 2;
static const Vec3f BALL_SHIFT = Vec3f(0, 0, 1);

struct Player {
    Ball ball;
    Racket racket;
    int lives = INIT_LIVES;
    bool hasReachedEndLine = false;
    bool hasLostALife = false;
    bool isReady = true;
    double recoverTime = 0;
    int bonus_picked = 0;

    Player();

    void spawn();
    void launchBall();
    void receiveBall();
    void setPosition(float x, float y);
    void loseALife();
    void recover(double delta_time);
    void makeAllCollisions(std::vector<Obstacle>& obstacles);

    void update(float delta_time);
};


