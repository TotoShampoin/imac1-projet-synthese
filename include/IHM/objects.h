#pragma once

#include "logic/vec2.h"
#include "IHM/base.h"

#define DEFAULT_FONT_OFFSET_C ' '
#define DEFAULT_FONT_NB_X 15
#define DEFAULT_FONT_NB_Y 8

struct Image {
	int width, height;
	GLenum format;
	GLuint texture_id;
	Image(const char* file);
    ~Image();
    void setFilter(GLint min, GLint mag);
};

struct Geometry {
    uint64_t vert_nb;
    GLfloat* vertices;  // * 3
    GLfloat* uv;        // * 2
    GLfloat* colors;    // * 4
    uint64_t tri_nb;
    GLuint* triangles;  // * 3

    Geometry(const char* file);
    Geometry(uint64_t vert_nb, uint64_t tri_nb);
    ~Geometry();

    // void draw();
};

struct Mesh {
    Geometry shape;
    Image texture;
};

struct Font {
    Image texture;
    float offset_u, offset_v, size_u, size_v;
    char offset_c;
    int nb_x, nb_y;

    Font(const char* texture_path, int offset_w, int offset_h, int size_w, int size_h, char first_character = DEFAULT_FONT_OFFSET_C, int nb_x = DEFAULT_FONT_NB_X, int nb_y = DEFAULT_FONT_NB_Y);

    Vec2f getCharacter(char);
};

void set_coord(float* table, int idx, float x, float y);
void set_coord(float* table, int idx, float x, float y, float z);
void set_coord(float* table, int idx, float x, float y, float z, float w);
void set_triangle(unsigned int* table, int idx, unsigned int t0, unsigned int t1, unsigned int t2);

Geometry createCube();
Geometry createWall();
Geometry createSphere(GLint precision);
Geometry createPlane();
