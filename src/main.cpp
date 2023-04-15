#include "main.h"

int main(int argc, const char* argv[]) {
    Window win {800, 600, "fenetre"};

    Image yey {"assets/yey.png"};
    Geometry cube = createCube();
    
    double timer = 0;
    while(!win.shouldClose()) {
        double startTime = glfwGetTime();

        win.clear();

    // Cube en 3D
        use3dMode(win);
        setCamera();
        
        drawGeometry(cube);

    // Image en 2D
        use2dMode(win);
		glPushMatrix();
            glTranslated(.5, 0, 0);
            glScaled(.25, .25, 1);
            glScalef(1, (float)yey.height/yey.width, 1);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            drawSquare(yey);
        glPopMatrix();

        win.refresh();
        win.pollEvents();

        double elapsedTime = glfwGetTime() - startTime;
        while(elapsedTime < F) {
            elapsedTime = glfwGetTime() - startTime;
        }
        timer += elapsedTime;
    }

    return 0;
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
