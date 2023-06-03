#include "IHM/draw.h"
#include "logic/vec3.h"

#include <cmath>
#include <cstring>

void draw3DObject(Geometry& geo, Image& img, Coord3D coords) {
    draw3DObject(geo, img, coords.position, coords.scale, coords.rotation_axis, coords.rotation_angle);
}
void draw3DObject(Geometry& geo, Coord3D coords) {
    draw3DObject(geo, coords.position, coords.scale, coords.rotation_axis, coords.rotation_angle);
}
void draw3DObject(Geometry& geo, Image& img, Vec3f vec, Vec3f scale, Vec3f rotation_axis, float rotation_angle) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, img.texture_id);
    draw3DObject(geo, vec, scale, rotation_axis, rotation_angle);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}
void draw3DObject(Geometry& geo, Vec3f vec, Vec3f scale, Vec3f rotation_axis, float rotation_angle) {
    glPushMatrix();
        glTranslated(vec.x, vec.y, vec.z);
        glScaled(scale.x, scale.y, scale.z);
        glRotated(rotation_angle, rotation_axis.x, rotation_axis.y, rotation_axis.z);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawGeometry(geo);
    glPopMatrix();
}

void draw2DBox(Vec2f pos, Vec2f scale, GLdouble rotation) {
    glPushMatrix();
        glTranslated(pos.x, pos.y, 0);
        glScaled(scale.x, scale.y, 1);
        glRotated(rotation, 0, 0, 1);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glBegin(GL_POLYGON);
            glVertex2f(-1, -1);
            glVertex2f( 1, -1);
            glVertex2f( 1,  1);
            glVertex2f(-1,  1);
        glEnd();
        glDisable(GL_BLEND);
    glPopMatrix();
}
void draw2DTexture(Image& img, Coord2D coords) {
    draw2DTexture(img, coords.position, coords.scale, coords.rotation_angle);
}
void draw2DTexture(Image& img, Vec2f pos, GLdouble scale, GLdouble rotation) {
    glPushMatrix();
        glTranslated(pos.x, pos.y, 0);
        glScaled(scale, scale, 1);
        glScalef(1, (float)img.height/img.width, 1);
        glRotated(rotation, 0, 0, 1);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawSquare(img);
    glPopMatrix();
}
void draw2DTexture(Image& img, Vec2f pos, Vec2f scale, GLdouble rotation) {
    glPushMatrix();
        glTranslated(pos.x, pos.y, 0);
        glScaled(scale.x, scale.y, 1);
        glScalef(1, (float)img.height/img.width, 1);
        glRotated(rotation, 0, 0, 1);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawSquare(img);
    glPopMatrix();
}

#include <cstdio>

void draw2DText(const char* text, Font& font, Coord2D coords, bool centered) {
    draw2DText(text, font, coords.position, coords.scale, coords.rotation_angle, centered);
}
void draw2DText(const char* text, Font& font, Vec2f pos, Vec2f scale, GLdouble rotation, bool centered) {
    if(!text) return;
    int len = strlen(text);
    Vec2f start_pos = Vec2f (
        -float(len)/2 + .5,
        0
    );
    if(!centered) {
        start_pos += Vec2f(float(len)/2, 1);
    }
    glPushMatrix();
        glTranslated(pos.x, pos.y, 0);
        glScaled(scale.x, scale.y, 1);
        glScalef((float)2*font.size_u/font.size_v, 2, 1);
        glRotated(rotation, 0, 0, 1);
        glTranslated(start_pos.x*2, start_pos.y, 0);
        for(int i = 0; i < len; i++) {
            Vec2f top_left = font.getCharacter(text[i]);
            Vec2f bottom_right = top_left + Vec2f(font.size_u, font.size_v);
            drawPartialSquare(font.texture, top_left, bottom_right);
            glTranslated(2, 0, 0);
        }
    glPopMatrix();
}

void drawGeometry(Geometry& geo) {
    glEnable(GL_BLEND);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3,GL_FLOAT, 0, geo.vertices);
    glColorPointer(4, GL_FLOAT, 0, geo.colors);
    glTexCoordPointer(2, GL_FLOAT, 0, geo.uv);
    glDrawElements(GL_TRIANGLES, 3*geo.tri_nb, GL_UNSIGNED_INT, geo.triangles);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_BLEND);
}

void drawSquare(Image& img) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, img.texture_id);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex2f(-1, -1);
        glTexCoord2f(1, 0);
        glVertex2f( 1, -1);
        glTexCoord2f(1, 1);
        glVertex2f( 1,  1);
        glTexCoord2f(0, 1);
        glVertex2f(-1,  1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}
void drawPartialSquare(Image& img, Vec2f top_left, Vec2f bottom_right) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, img.texture_id);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_POLYGON);
        glTexCoord2f(top_left.x, top_left.y);
        glVertex2f(-1, -1);
        glTexCoord2f(bottom_right.x, top_left.y);
        glVertex2f( 1, -1);
        glTexCoord2f(bottom_right.x, bottom_right.y);
        glVertex2f( 1,  1);
        glTexCoord2f(top_left.x, bottom_right.y);
        glVertex2f(-1,  1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

float toRad(float deg) {
    return deg*M_PI/180.0f;
}

void cameraToCenter() {
	gluLookAt(0.0, 0.0, 0.0,
        0.0,0.0,-1.0,
    0.0,-1.0,0.0);
}

void setCamera(float theta, float phy, float dist_zoom) {
	gluLookAt(dist_zoom*cos(toRad(theta))*sin(toRad(phy)),
			  dist_zoom*sin(toRad(theta))*sin(toRad(phy)),
			  dist_zoom*cos(toRad(phy)),
			  0.0,0.0,0.0,
			  0.0,0.0,1.0);
}

void use3dMode(Window& win) {
    win.updateSize();
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, win.width, win.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, win.aspect_ratio, .1, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void use2dMode(Window& win) {
    win.updateSize();
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, win.width, win.height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    // // carré étiré
    // gluOrtho2D(-1, 1, 1, -1);
    gluOrtho2D(
    -GL_VIEW_SIZE / 2. * win.aspect_ratio, GL_VIEW_SIZE / 2. * win.aspect_ratio,
    GL_VIEW_SIZE / 2., -GL_VIEW_SIZE / 2.);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
