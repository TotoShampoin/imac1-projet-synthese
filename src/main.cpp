#include "main.h"
#include "IHM/draw.h"

int main(int argc, const char* argv[]) {
    Window win {800, 600, "fenetre"};

    Image yey {"assets/yey.png"};
    Geometry cube = createCube();
    Geometry sphere = createSphere(64);
    
    double timer = 0;
    while(!win.shouldClose()) {
        double startTime = glfwGetTime();

        win.clear();

    // Cube en 3D
        use3dMode(win);
        setCamera(0, 90, 0.1);
        
        draw3DObject(sphere, Vec3f(5 * cos(5 * timer), 5 * sin(5 * timer), 0));

    // Image en 2D
        use2dMode(win);
		//draw2DTexture(yey, .5, 0, .25);

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
