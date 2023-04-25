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

    Geometry(unsigned long vert_nb, unsigned long tri_nb);
    ~Geometry();

    // void draw();
};

struct Object {
    Image&& texture;
    Geometry&& forme;
};
