#include "main.h"
#include "IHM/draw.h"
#include "physics.h"

#include "game/ball.h"
#include "game/level.h"

void drawWorld(Window& win, Ball& ball) {
static Mesh ball_mesh = makeBallMesh();
static Mesh wall_mesh = makeWallMesh();

    use3dMode(win);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glColor4f(1,1,1,1);

    gluLookAt(
        0, 0, 0,
        0, 0, 1,
        0, 1, 0
    );
    
    draw3DObject(
        ball_mesh.shape, ball_mesh.texture, 
        ball.position + Vec3f(0,0,5),
        Vec3f(1,1,1) * ball.radius
    );
    for (int i = -1; i < 20; i++) {
        draw3DObject(
            wall_mesh.shape, wall_mesh.texture,
            Vec3f(0, 0, 3 + 2*i),
            Vec3f(1, -1, 1)
        );
    }
}


int main(int argc, const char* argv[]) {
    Window win {800, 600, "fenetre"};

    Ball ball;
    ball.position = Vec3f(0, .5, 0);
    ball.speed = Vec3f(1, 1, 0).normalize();

    double timer = 0;
    double deltaTime = 0;
    while(!win.shouldClose()) {
        double startTime = glfwGetTime();

        ball.move(deltaTime);

        win.clear();
        drawWorld(win, ball);

    // Image en 2D
        use2dMode(win);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glColor4f(1,1,1,1);
        
        win.refresh();
        win.pollEvents();

        deltaTime = glfwGetTime() - startTime;
        while(deltaTime < F) {
            deltaTime = glfwGetTime() - startTime;
        }
        timer += deltaTime;
    }

    return 0;
}
