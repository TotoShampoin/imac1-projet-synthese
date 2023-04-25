#include "main.h"
#include "IHM/draw.h"
#include "physics.h"

int main(int argc, const char* argv[]) {
    Window win {800, 600, "fenetre"};

    Image yes {"assets/check.png"};
    Image no  {"assets/cross.png"};
    
    Image yey {"assets/yey.png"};
    Geometry cube = createCube();
    Geometry sphere = createSphere(24);

    PhysicsSphere ball { Vec3f(2, 0, 0), .5 };
    PhysicsAABB box { Vec3f(0, -1, -2) , Vec3f(-2, 1, -.45) };
    
    bool is_touching = false;
    
    double timer = 0;
    while(!win.shouldClose()) {
        double startTime = glfwGetTime();

        ball.position.x = cos(timer) + 1;

        is_touching = ball.collide(box);
        
        Vec3f radius_box = Vec3f(1,1,1) * ball.radius;
        PhysicsAABB sphere_box = PhysicsAABB(
            ball.position - radius_box,
            ball.position + radius_box
        );

        win.clear();

    // Cube en 3D
        use3dMode(win);

        // // Test texture
        // cameraToCenter();
        // glTranslatef(0, -10, 0);
        // glRotatef(timer * 180 / M_PI, 1, 1, 1);
        // draw3DObject(cube);

        // Test collision
        setCamera(90, 90, 5);
        
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glColor4f(0,.5,1,1);
        draw3DObject(cube,
            (box.boundA+box.boundB)/2,
            box.boundB - (box.boundA+box.boundB)/2
        );
        
        glColor4f(1,1,0,1);
        draw3DObject(sphere, ball.position, Vec3f(1,1,1) * ball.radius);

        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glColor4f(1,0,0,1);
        draw3DObject(cube,
            (sphere_box.boundA+sphere_box.boundB)/2,
            sphere_box.boundB - (sphere_box.boundA+sphere_box.boundB)/2
        );

    // Image en 2D
        use2dMode(win);
        glColor4f(1,1,1,1);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		// draw2DTexture(yey, .5, 0, .25);

        draw2DTexture(is_touching ? yes : no, .75, .75, .125);
        
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
