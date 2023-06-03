#pragma once

#include "IHM/draw.h"
#include "logic/physics.h"

const int FPS = 60;
const double FRAME_TIME = 1./FPS;

float clamp(float t, float min, float max);
float lerp(float a, float b, float t);
float invlerp(float v, float a, float b);
float remap(float t, float t_start, float t_end, float a, float b);
void drawAABB(PhysicsAABB& box);
double timer();
bool endsWith(const char* str,  const char* endstr);
