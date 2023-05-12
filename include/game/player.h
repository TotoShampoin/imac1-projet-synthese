#pragma once

#include "IHM/window.h"
#include "IHM/objects.h"
#include "logic/vec3.h"
#include "game/ball.h"
#include "game/racket.h"

#include <cmath>
#include <iostream>

struct Player {
    Ball ball;
    Racket racket;

    void spawn();
    void launchBall();
    void receiveBall();
};


