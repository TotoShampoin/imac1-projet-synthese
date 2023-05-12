#pragma once

#include "IHM/base.h"

struct Image {
	int width, height;
	GLenum format;
	GLuint texture_id;
	Image(const char* file);
    ~Image();
};

struct Geometry {
    unsigned long vert_nb;
    GLfloat* vertices;  // * 3
    GLfloat* uv;        // * 2
    GLfloat* colors;    // * 4
    unsigned long tri_nb;
    GLuint* triangles;  // * 3

    Geometry(const char* file);
    Geometry(unsigned long vert_nb, unsigned long tri_nb);
    ~Geometry();

    // void draw();
};

struct Mesh {
    Geometry shape;
    Image texture;
};

void set_coord(float* table, int idx, float x, float y);
void set_coord(float* table, int idx, float x, float y, float z);
void set_coord(float* table, int idx, float x, float y, float z, float w);
void set_triangle(unsigned int* table, int idx, unsigned int t0, unsigned int t1, unsigned int t2);

Geometry createCube();
Geometry createWall();
Geometry createSphere(GLint precision);
