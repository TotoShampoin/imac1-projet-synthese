#pragma once

#include "IHM/window.h"
#include "IHM/objects.h"
#include "logic/vec3.h"
#include "game/ball.h"
#include "game/racket.h"

#include <cmath>
#include <iostream>

struct Player {
    Racket racket;
    Vec3f position;
    Ball ball;

    void spawn();
};


