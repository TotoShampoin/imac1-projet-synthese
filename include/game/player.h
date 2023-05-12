#pragma once

#include "IHM/window.h"
#include "IHM/objects.h"
#include "logic/vec3.h"

#include <cmath>
#include <iostream>

struct Ball {
    Geometry& sphere;
    // Bonus
};

struct Racket {
    Geometry& rect;
    // Bonus
};

struct Player {
    Racket racket;
    Vec3f position;
    Ball ball;

    void spawn();
};


