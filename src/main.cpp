#include "main.h"
#include "IHM/draw.h"

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
		draw2DTexture(yey, .5, 0, .25);

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
