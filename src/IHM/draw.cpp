#include "IHM/draw.h"

#include <cmath>

void drawGeometry(Geometry& geo) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3,GL_FLOAT, 0, geo.vertices);
    glDrawElements(GL_TRIANGLES, 3*geo.tri_nb, GL_UNSIGNED_INT, geo.triangles);
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

void setCamera(float theta, float phy, float dist_zoom) {
	gluLookAt(dist_zoom*cos(toRad(theta))*sin(toRad(phy)),
			  dist_zoom*sin(toRad(theta))*sin(toRad(phy)),
			  dist_zoom*cos(toRad(phy)),
			  0.0,0.0,0.0,
			  0.0,0.0,1.0);
}

void use3dMode(Window& win) {
    win.updateSize();
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
