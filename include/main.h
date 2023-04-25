#include "IHM/window.h"
#include "IHM/objects.h"

#include <cmath>
#include <iostream>


const int FPS = 60;
const double F = 1./FPS;

void set_coord(float* table, int idx, float x, float y);
void set_coord(float* table, int idx, float x, float y, float z);
void set_coord(float* table, int idx, float x, float y, float z, float w);
void set_triangle(unsigned int* table, int idx, unsigned int t0, unsigned int t1, unsigned int t2);

Geometry createCube();
