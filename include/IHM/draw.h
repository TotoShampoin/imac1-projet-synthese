#pragma once

#include "IHM/window.h"
#include "IHM/objects.h"
#include "logic/vec2.h"
#include "logic/vec3.h"

struct Coord3D {
    Vec3f position;
    Vec3f scale;
    Vec3f rotation_axis;
    float rotation_angle;
};
struct Coord2D {
    Vec2f position;
    Vec2f scale;
    float rotation_angle;
};

const double GL_VIEW_SIZE = 2.;

void draw3DObject(Geometry& geo, Image& img, Coord3D coords);
void draw3DObject(Geometry& geo, Coord3D coords);
void draw3DObject(Geometry& geo, Image& img, Vec3f vec = Vec3f(0, 0, 0), Vec3f scale = Vec3f(1, 1, 1), Vec3f rotation_axis = Vec3f(1, 0, 0), float rotation_angle = 0);
void draw3DObject(Geometry& geo, Vec3f vec = Vec3f(0, 0, 0), Vec3f scale = Vec3f(1, 1, 1), Vec3f rotation_axis = Vec3f(1, 0, 0), float rotation_angle = 0);

void draw2DBox(Vec2f pos, Vec2f scale, GLdouble rotation = 0);
void draw2DTexture(Image& img, Coord2D coords);
void draw2DTexture(Image& img, Vec2f pos, GLdouble scale = 1., GLdouble rotation = 0.);
void draw2DTexture(Image& img, Vec2f pos, Vec2f scale = Vec2f(1, 1), GLdouble rotation = 0.);
void draw2DText(const char* text, Font& font, Coord2D coords);
void draw2DText(const char* text, Font& font, Vec2f pos, Vec2f scale = Vec2f(1, 1), GLdouble rotation = 0.);

void drawGeometry(Geometry& geo);
void drawSquare(Image& img);
void drawPartialSquare(Image& img, Vec2f top_left, Vec2f bottom_right);

float toRad(float deg);

void cameraToCenter();
void setCamera(float theta = 45.0f, float phy = 60.0f, float dist_zoom = 30.0f);
void use3dMode(Window& win);
void use2dMode(Window& win);
