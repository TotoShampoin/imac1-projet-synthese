#pragma once

#include "IHM/draw.h"
#include "logic/physics.h"

const int FPS = 60;
const double FRAME_TIME = 1./FPS;

float clamp(float t, float min, float max);
void drawAABB(PhysicsAABB& box);
double timer();
