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
