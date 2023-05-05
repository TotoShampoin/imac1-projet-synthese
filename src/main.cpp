#include "main.h"
#include "IHM/draw.h"
#include "physics.h"

void drawAABB(PhysicsAABB& box, Image& texture) {
    static Geometry geo = createCube();
    Vec3f center = box.middle();
    Vec3f size = box.size();
    draw3DObject(geo, texture, center, size);
}
void drawSphere(PhysicsSphere& sph) {
    static Geometry geo = createSphere(24);
    Vec3f size = Vec3f(1,1,1) * sph.radius;
    draw3DObject(geo, sph.position, size);
}
void drawLine(Vec3f l1, Vec3f l2) {
    glBegin(GL_LINES);
        glVertex3f(l1.x,l1.y,l1.z);
        glVertex3f(l2.x,l2.y,l2.z);
    glEnd();
}

int main(int argc, const char* argv[]) {
    Window win {800, 600, "fenetre"};

    Image cube_texture {"assets/corridor.png"};
    Image wall_texture {"assets/walls.png"};
    Image transparent {"assets/wallcube.png"};
    Geometry cube = createCube();
    Geometry wall = createWall();
    Geometry sphere = createSphere(24);

    PhysicsAABB box {
        Vec3f(-1, -1, -1),
        Vec3f(1, 1, 1),
    };
    PhysicsSphere ball { Vec3f(1, 0, 0) , .5 };
    
    double timer = 0;
    while(!win.shouldClose()) {
        double startTime = glfwGetTime();
        win.clear();

        ball.position.x = cos(timer) * 1.75;
        ball.position.y = sin(timer) * 1.25;
        
        Vec3f normal {0,0,0};
        ball.collide(box, normal);

    // Cube en 3D
        use3dMode(win);

        // // Test texture
        // gluLookAt(
        //     1, 4, 3,
        //     0, 0, 0,
        //     0, 0, 1
        // );
        // glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        // glPushMatrix();
        // glRotatef(timer * 180 / M_PI, 1, 0, 0);
        // draw3DObject(cube, cube_texture, Vec3f(0, 0, 2.5)); // derrière
        // draw3DObject(cube, transparent, Vec3f(0, 0, 0)); // devant
        // glPopMatrix();

        // // Test collision
        // gluLookAt(
        //     3, 4, 3,
        //     0, 0, 0,
        //     0, 0, 1
        // );
        // glColor4f(1,1,1,1);
        // glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        // glPushMatrix();
        // glRotatef(timer * 180 / M_PI, 1, 0, 0);
        // drawAABB(box, cube_texture);
        // glPopMatrix();

        // Test level
        gluLookAt(
            0, 0, 3,
            0, 0, 0,
            0, 1, 1
        );
        glColor4f(1,1,1,1);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        for (int i = -1; i < 20; i++) {
            draw3DObject(wall, wall_texture, Vec3f(0, 0, -2 * i), Vec3f(-1, -1, -1));
        }
        
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        drawSphere(ball);

        glColor4f(1,0,0,1);
        drawLine(ball.position, ball.position + normal);

        // std::cout << "(" << normal.x << ", " <<  normal.y << ", " <<  normal.z << ")" << std::endl;
        
    // Image en 2D
        use2dMode(win);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glColor4f(1,1,1,1);
        
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
