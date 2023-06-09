#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iostream>

#include "IHM/objects.h"

#define file_read(file, data) file.read(reinterpret_cast<char*>(&data), sizeof(data))
#define file_write(file, data) file.write(reinterpret_cast<char*>(&data), sizeof(data))


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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

void Image::setFilter(GLint min, GLint mag) {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Geometry::Geometry(uint64_t verts, uint64_t tris) {
    vert_nb = verts;
    tri_nb = tris;
    vertices = new float[vert_nb * 3];
    uv = new float[vert_nb * 2];
    colors = new float[vert_nb * 4];
    triangles = new unsigned int[tri_nb * 3];
}
Geometry::Geometry(const char* file_path) {
	std::fstream file (file_path, std::ios::in | std::fstream::binary);
	if(!file) {
		throw std::runtime_error(std::string(file_path) + " not found");
	}

	file_read(file, vert_nb);
	file_read(file, tri_nb);

	vertices = new GLfloat[vert_nb*3];
	uv = new GLfloat[vert_nb*2];
    colors = new float[vert_nb * 4];
	triangles = new GLuint[tri_nb*3];

	for(size_t i = 0; i < vert_nb * 3; i++) {
		file_read(file, vertices[i]);
	}
	for(size_t i = 0; i < vert_nb * 2; i++) {
		file_read(file, uv[i]);
	}
	for(size_t i = 0; i < vert_nb * 4; i++) {
		colors[i] = 1;
	}
	for(size_t i = 0; i < tri_nb * 3; i++) {
		file_read(file, triangles[i]);
	}
}

Geometry::~Geometry() {
    delete[] vertices;
    delete[] uv;
    delete[] colors;
    delete[] triangles;
}

Font::Font(const char* texture_path, int offset_w, int offset_h, int size_w, int size_h, char first_character, int nx, int ny):
    texture(Image(texture_path))
{
    offset_u = float(offset_w) / texture.width;
    offset_v = float(offset_h) / texture.height;
    size_u   = float(size_w)   / texture.width;
    size_v   = float(size_h)   / texture.height;
    offset_c = first_character;
    nb_x = nx;
    nb_y = ny;
}

Vec2f Font::getCharacter(char c) {
    char oc = c - offset_c;
    return Vec2f (
        offset_u * (oc % nb_x),
        offset_v * ((oc / nb_x) % nb_y)
    );
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
    Geometry cube (24, 12);

    /* VERTICES */
    // ABCD / DIE 1
    set_coord(cube.vertices, 0, -1, -1,  1); // A
    set_coord(cube.vertices, 1,  1, -1,  1); // B
    set_coord(cube.vertices, 2,  1,  1,  1); // C
    set_coord(cube.vertices, 3, -1,  1,  1); // D
    // EFGH / DIE 6
    set_coord(cube.vertices, 4, -1,  1, -1); // E
    set_coord(cube.vertices, 5,  1,  1, -1); // F
    set_coord(cube.vertices, 6,  1, -1, -1); // G
    set_coord(cube.vertices, 7, -1, -1, -1); // H
    // BGFC / DIE 2
    set_coord(cube.vertices, 8,  1, -1,  1); // B
    set_coord(cube.vertices, 9,  1, -1, -1); // G
    set_coord(cube.vertices,10,  1,  1, -1); // F
    set_coord(cube.vertices,11,  1,  1,  1); // C
    // HADE / DIE 5
    set_coord(cube.vertices,12, -1, -1, -1); // H
    set_coord(cube.vertices,13, -1, -1,  1); // A
    set_coord(cube.vertices,14, -1,  1,  1); // D
    set_coord(cube.vertices,15, -1,  1, -1); // E
    // HGBA / DIE 3
    set_coord(cube.vertices,16, -1, -1, -1); // H
    set_coord(cube.vertices,17,  1, -1, -1); // G
    set_coord(cube.vertices,18,  1, -1,  1); // B
    set_coord(cube.vertices,19, -1, -1,  1); // A
    // DCFE / DIE 4
    set_coord(cube.vertices,20, -1,  1,  1); // D
    set_coord(cube.vertices,21,  1,  1,  1); // C
    set_coord(cube.vertices,22,  1,  1, -1); // F
    set_coord(cube.vertices,23, -1,  1, -1); // E

    /* UV MAP*/
    // ABCD / DIE 1
    set_coord(cube.uv, 0,    0,    0); // A
    set_coord(cube.uv, 1, 1/3.,    0); // B
    set_coord(cube.uv, 2, 1/3., 1/2.); // C
    set_coord(cube.uv, 3,    0, 1/2.); // D
    // EFGH / DIE 6
    set_coord(cube.uv, 4, 2/3., 1/2.); // E
    set_coord(cube.uv, 5,    1, 1/2.); // F
    set_coord(cube.uv, 6,    1,    1); // G
    set_coord(cube.uv, 7, 2/3.,    1); // H
    // BGFC / DIE 2
    set_coord(cube.uv, 8, 1/3.,    0); // B
    set_coord(cube.uv, 9, 2/3.,    0); // G
    set_coord(cube.uv,10, 2/3., 1/2.); // F
    set_coord(cube.uv,11, 1/3., 1/2.); // C
    // HADE / DIE 5
    set_coord(cube.uv,12, 1/3., 1/2.); // H
    set_coord(cube.uv,13, 2/3., 1/2.); // A
    set_coord(cube.uv,14, 2/3.,    1); // D
    set_coord(cube.uv,15, 1/3.,    1); // E
    // HGBA / DIE 3
    set_coord(cube.uv,16, 2/3.,    0); // H
    set_coord(cube.uv,17,    1,    0); // G
    set_coord(cube.uv,18,    1, 1/2.); // B
    set_coord(cube.uv,19, 2/3., 1/2.); // A
    // DCFE / DIE 4
    set_coord(cube.uv,20,    0, 1/2.); // D
    set_coord(cube.uv,21, 1/3., 1/2.); // C
    set_coord(cube.uv,22, 1/3.,    1); // F
    set_coord(cube.uv,23,    0,    1); // E

    for(int i = 0; i < 6; i++) {
        set_triangle(cube.triangles, i*2+0, i*4, i*4+1, i*4+2);
        set_triangle(cube.triangles, i*2+1, i*4, i*4+2, i*4+3);
    }

    for(int i = 0; i < 24; i++) {
        set_coord(cube.colors, i, 1, 1, 1, 1);
    }

    return cube;
}

Geometry createWall() {
    Geometry cube (16, 8);

    /* VERTICES */
    // ABGH / TOP
    set_coord(cube.vertices, 0, -1, -1,  1); // A
    set_coord(cube.vertices, 1,  1, -1,  1); // B
    set_coord(cube.vertices, 2,  1, -1, -1); // G
    set_coord(cube.vertices, 3, -1, -1, -1); // H
    // BCFG / LEFT
    set_coord(cube.vertices, 4,  1, -1,  1); // B
    set_coord(cube.vertices, 5,  1,  1,  1); // C
    set_coord(cube.vertices, 6,  1,  1, -1); // F
    set_coord(cube.vertices, 7,  1, -1, -1); // G
    // CDEF / BOTTOM
    set_coord(cube.vertices, 8,  1,  1,  1); // C
    set_coord(cube.vertices, 9, -1,  1,  1); // D
    set_coord(cube.vertices,10, -1,  1, -1); // E
    set_coord(cube.vertices,11,  1,  1, -1); // F
    // DAHE / RIGHT
    set_coord(cube.vertices,12, -1,  1,  1); // D
    set_coord(cube.vertices,13, -1, -1,  1); // A
    set_coord(cube.vertices,14, -1, -1, -1); // H
    set_coord(cube.vertices,15, -1,  1, -1); // E

    /* UV MAP*/
    // ABGH / TOP
    set_coord(cube.uv, 0,    0, 0); // A
    set_coord(cube.uv, 1, 1/4., 0); // B
    set_coord(cube.uv, 2, 1/4., 1); // G
    set_coord(cube.uv, 3,    0, 1); // H
    // BCFG / LEFT
    set_coord(cube.uv, 4, 1/4., 0); // B
    set_coord(cube.uv, 5, 2/4., 0); // C
    set_coord(cube.uv, 6, 2/4., 1); // F
    set_coord(cube.uv, 7, 1/4., 1); // G
    // CDEF / BOTTOM
    set_coord(cube.uv, 8, 2/4., 0); // C
    set_coord(cube.uv, 9, 3/4., 0); // D
    set_coord(cube.uv,10, 3/4., 1); // E
    set_coord(cube.uv,11, 2/4., 1); // F
    // DAHE / RIGHT
    set_coord(cube.uv,12, 3/4., 0); // D
    set_coord(cube.uv,13,    1, 0); // A
    set_coord(cube.uv,14,    1, 1); // H
    set_coord(cube.uv,15, 3/4., 1); // E

    for(int i = 0; i < 4; i++) {
        set_triangle(cube.triangles, i*2+0, i*4, i*4+1, i*4+2);
        set_triangle(cube.triangles, i*2+1, i*4, i*4+2, i*4+3);
    }

    for(int i = 0; i < 16; i++) {
        set_coord(cube.colors, i, 1, 1, 1, 1);
    }

    return cube;
}

Geometry createSphere(GLint precision) {
    int vertex_number = precision * (precision / 2 + 1); // A CHANGER
    int triangle_number = vertex_number * 2; // A CHANGER

    Geometry sphere (vertex_number, triangle_number);

    int points_i = precision / 2 + 1;
    int points_j = precision;
    int a, b, c, d;
    for (int i = 0; i < points_i; i++) { // LATITUDE
        for (int j = 0; j < points_j; j++) { // LONGITUDE
            a = i * points_j + j;
            b = i * points_j + ((j + 1) % points_j);
            c = ((i + 1) % points_i) * points_j + j;
            d = ((i + 1) % points_i) * points_j + ((j + 1) % points_j);

            float _i = 2 * M_PI * (float)i / precision;
            float _j = 2 * M_PI * (float)j / precision;

            float x = cos(_j)*sin(_i);
            float z = sin(_j)*sin(_i);
            float y = cos(_i);

            set_coord(sphere.vertices, a, x, y, z);
            set_coord(sphere.colors, a, 1, 1, 1, 1);
            set_coord(sphere.uv, a, (float)j/(points_j-1), (float)i/(points_i-1));
            set_triangle(sphere.triangles, 2 * a, a, b, d);
            set_triangle(sphere.triangles, 2 * a + 1, a, c, d);
        }
    }

    return sphere;
}

Geometry createPlane() {
    Geometry plane (4, 2);

    set_coord(plane.vertices, 0, -1, -1,  0);
    set_coord(plane.vertices, 1,  1, -1,  0);
    set_coord(plane.vertices, 2,  1,  1,  0);
    set_coord(plane.vertices, 3, -1,  1,  0);

    set_coord(plane.uv, 0, 0, 0);
    set_coord(plane.uv, 1, 1, 0);
    set_coord(plane.uv, 2, 1, 1);
    set_coord(plane.uv, 3, 0, 1);

    for(int i = 0; i < 4; i++) {
        set_coord(plane.colors, i, 1, 1, 1, 1);
    }
    set_triangle(plane.triangles, 0, 0, 1, 2);
    set_triangle(plane.triangles, 1, 0, 2, 3);

    return plane;
}
