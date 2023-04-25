#pragma once

#include "IHM/window.h"
#include "IHM/objects.h"

const double GL_VIEW_SIZE = 2.;

void drawGeometry(Geometry& geo);
void drawSquare(Image& img);

float toRad(float deg);
void setCamera(float theta = 45.0f, float phy = 60.0f, float dist_zoom = 30.0f);
void use3dMode(Window& win);
void use2dMode(Window& win);