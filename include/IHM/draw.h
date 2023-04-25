#pragma once

#include "IHM/window.h"
#include "IHM/objects.h"
#include "vec3.h"

const double GL_VIEW_SIZE = 2.;

void draw3DObject(Geometry& geo, Vec3f vec, Vec3f scale = Vec3f(1, 1, 1));
void draw2DTexture(Image& img, GLdouble pos_x, GLdouble pos_y, GLdouble scale = 1.);
void draw2DTexture(Image& img, GLdouble pos_x, GLdouble pos_y, GLdouble scale_x, GLdouble scale_y);
void drawGeometry(Geometry& geo);
void drawSquare(Image& img);

float toRad(float deg);
void setCamera(float theta = 45.0f, float phy = 60.0f, float dist_zoom = 30.0f);
void use3dMode(Window& win);
void use2dMode(Window& win);