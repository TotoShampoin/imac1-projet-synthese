#include "IHM/window.h"
#include "IHM/objects.h"

#include <cmath>
#include <iostream>


const int FPS = 60;
const double F = 1./FPS;
const double GL_VIEW_SIZE = 2.;

void set_coord(float* table, int idx, float x, float y);
void set_coord(float* table, int idx, float x, float y, float z);
void set_coord(float* table, int idx, float x, float y, float z, float w);
void set_triangle(unsigned int* table, int idx, unsigned int t0, unsigned int t1, unsigned int t2);

Geometry createCube();
void drawGeometry(Geometry& geo);
void drawSquare(Image& img);

float toRad(float deg);
void setCamera(float theta = 45.0f, float phy = 60.0f, float dist_zoom = 30.0f);
void use3dMode(Window& win);
void use2dMode(Window& win);