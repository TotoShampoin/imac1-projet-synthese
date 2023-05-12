#include "main.h"
#include "IHM/draw.h"
#include "logic/physics.h"

#include "game/ball.h"
#include "game/level.h"

void drawAABB(PhysicsAABB& box) {
    static Geometry geo = createCube();
    Vec3f center = box.middle();
    Vec3f size = box.size();
    draw3DObject(geo, center, size);
}

void drawWorld(Window& win, Ball& ball, Level& level) {
static Mesh ball_mesh = makeBallMesh();
static Mesh wall_mesh = makeWallMesh();

    use3dMode(win);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    gluLookAt(
        0, 0, 0,
        0, 0, 1,
        0, 1, 0
    );
    
    draw3DObject(
        ball_mesh.shape, ball_mesh.texture, 
        ball.position + Vec3f(0,0,0),
        Vec3f(1,1,1) * ball.radius
    );
    int size = level.length/2;
    for (int i = 0; i < size; i++) {
        draw3DObject(
            wall_mesh.shape, wall_mesh.texture,
            Vec3f(0, 0, 2*(i+1)),
            Vec3f(1, -1, 1)
        );
    }
    
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    for(auto& box : level.obstacles) {
        PhysicsAABB tmp_box {
            Vec3f(box.position - box.size),
            Vec3f(box.position + box.size),
        };
        drawAABB(tmp_box);
    }

}


int main(int argc, const char* argv[]) {
    Window win {800, 600, "fenetre"};

    Level level;

    Ball ball;
    ball.position = Vec3f(-.75, 0, 2);
    ball.speed = Vec3f(1, 1, 2);

    double timer = 0;
    double deltaTime = 0;
    while(!win.shouldClose()) {
        double startTime = glfwGetTime();

        ball.move(deltaTime);
        ball.collide(level.obstacles);

        win.clear();
        drawWorld(win, ball, level);

    // Image en 2D
        use2dMode(win);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        
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
