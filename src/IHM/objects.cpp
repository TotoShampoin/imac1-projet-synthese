#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>

#include "IHM/objects.h"

Image::Image(const char* file) {
    int channels_in_file;
    stbi_uc* img = stbi_load(file, &width, &height, &channels_in_file, 0);
    if(!img) {
        std::ostringstream msg;
        msg << "File " << file << " does not exist";
        throw std::runtime_error(msg.str());
    }
    switch(channels_in_file) {
        case 1: format = GL_RED; break;
        case 2: format = GL_RG; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: format = 0;
    }
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D, 0, format,
        width, height, 0, format,
        GL_UNSIGNED_BYTE, img
    );
    stbi_image_free(img);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Image::~Image() {
    glDeleteTextures(1, &texture_id);
}

Geometry::Geometry(unsigned long verts, unsigned long tris) {
    vert_nb = verts;
    tri_nb = tris;
    vertices = new float[vert_nb * 3];
    uv = new float[vert_nb * 2];
    colors = new float[vert_nb * 4];
    triangles = new unsigned int[tri_nb * 3];
}

Geometry::~Geometry() {
    delete[] vertices;
    delete[] uv;
    delete[] colors;
    delete[] triangles;
}

void set_coord(float* table, int idx, float x, float y) {
	table[2 * idx + 0] = x;
	table[2 * idx + 1] = y;
}
void set_coord(float* table, int idx, float x, float y, float z) {
	table[3 * idx + 0] = x;
	table[3 * idx + 1] = y;
	table[3 * idx + 2] = z;
}
void set_coord(float* table, int idx, float x, float y, float z, float w) {
	table[4 * idx + 0] = x;
	table[4 * idx + 1] = y;
	table[4 * idx + 2] = z;
	table[4 * idx + 3] = w;
}
void set_triangle(unsigned int* table, int idx, unsigned int t0, unsigned int t1, unsigned int t2) {
	table[3 * idx + 0] = t0;
	table[3 * idx + 1] = t1;
	table[3 * idx + 2] = t2;
}

Geometry createCube() {
    Geometry cube (8, 12);

    set_coord(cube.vertices, 0, -1, -1, -1); // A
    set_coord(cube.vertices, 1,  1, -1, -1); // B
    set_coord(cube.vertices, 2, -1,  1, -1); // C
    set_coord(cube.vertices, 3,  1,  1, -1); // D
    set_coord(cube.vertices, 4, -1, -1,  1); // E
    set_coord(cube.vertices, 5,  1, -1,  1); // F
    set_coord(cube.vertices, 6, -1,  1,  1); // G
    set_coord(cube.vertices, 7,  1,  1,  1); // H
    set_triangle(cube.triangles, 0, 0, 1, 2); // A B C
    set_triangle(cube.triangles, 1, 1, 2, 3); // B C D
    set_triangle(cube.triangles, 2, 4, 5, 6); // E F G
    set_triangle(cube.triangles, 3, 5, 6, 7); // F G H
    set_triangle(cube.triangles, 4, 0, 4, 6); // A E G
    set_triangle(cube.triangles, 5, 0, 2, 6); // A C G
    set_triangle(cube.triangles, 6, 3, 1, 5); // D B F
    set_triangle(cube.triangles, 7, 3, 7, 5); // D H F
    set_triangle(cube.triangles, 8, 0, 4, 5); // A E F
    set_triangle(cube.triangles, 9, 0, 1, 5); // A B F
    set_triangle(cube.triangles, 10, 2, 6, 7); // C G H
    set_triangle(cube.triangles, 11, 2, 3, 7); // C D H

    return cube;
}

Geometry createSphere(GLint precision) {
    Geometry sphere (8, 12);

    return sphere;
}
