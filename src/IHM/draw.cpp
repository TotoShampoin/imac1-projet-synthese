#include "IHM/draw.h"
#include "logic/vec3.h"

#include <cmath>

void draw3DObject(Geometry& geo, Image& img, Vec3f vec, Vec3f scale) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, img.texture_id);
    draw3DObject(geo, vec, scale);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void draw3DObject(Geometry& geo, Vec3f vec, Vec3f scale) {
    glPushMatrix();
        glTranslated(vec.x, vec.y, vec.z);
        glScaled(scale.x, scale.y, scale.z);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawGeometry(geo);
    glPopMatrix();
}

void draw2DTexture(Image& img, GLdouble pos_x, GLdouble pos_y, GLdouble scale) {
    glPushMatrix();
        glTranslated(pos_x, pos_y, 0);
        glScaled(scale, scale, 1);
        glScalef(1, (float)img.height/img.width, 1);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawSquare(img);
    glPopMatrix();
}

void draw2DTexture(Image& img, GLdouble pos_x, GLdouble pos_y, GLdouble scale_x, GLdouble scale_y) {
    glPushMatrix();
        glTranslated(pos_x, pos_y, 0);
        glScaled(scale_x, scale_y, 1);
        glScalef(1, (float)img.height/img.width, 1);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawSquare(img);
    glPopMatrix();
}

void drawGeometry(Geometry& geo) {
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
