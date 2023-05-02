#include "main.h"
#include "IHM/draw.h"
#include "physics.h"

int main(int argc, const char* argv[]) {
    Window win {800, 600, "fenetre"};

    Image wall {"assets/corridor.png"};
    Geometry cube = createCube();
    Geometry sphere = createSphere(24);
    
    bool is_touching = false;
    
    double timer = 0;
    while(!win.shouldClose()) {
        double startTime = glfwGetTime();
        win.clear();

    // Cube en 3D
        use3dMode(win);

        // // Test texture
        gluLookAt(
            0, 0, -3,
            0, 0, 0,
            0, -1, 0
        );
        
        draw3DObject(cube, wall, Vec3f(0, 0, 0)); // devant
        draw3DObject(cube, wall, Vec3f(0, 0, 2)); // derrière
        
    // Image en 2D
        use2dMode(win);
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
