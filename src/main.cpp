#include "main.h"
#include "IHM/draw.h"
#include "logic/physics.h"

#include "game/ball.h"
#include "game/player.h"

float clamp(float t, float min, float max) {
    return (t < min) ? min : (t > max) ? max : t;
}

void drawAABB(PhysicsAABB& box) {
    static Geometry geo = createCube();
    Vec3f center = box.middle();
    Vec3f size = box.size();
    draw3DObject(geo, center, size);
}

void drawWorld(Window& win, Player& player, Level& level) {
static Mesh ball_mesh = makeBallMesh();
static Mesh wall_mesh = makeWallMesh();
static Geometry racket_mesh = makeRacketMesh();

    use3dMode(win);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    gluLookAt(
        0, 0, 0,
        0, 0, 1,
        0, 1, 0
    );
    
    draw3DObject(
        ball_mesh.shape, ball_mesh.texture, 
        player.ball.position + Vec3f(0,0,0),
        Vec3f(1,1,1) * player.ball.radius
    );
    int size = level.length/2;
    for (int i = 0; i < size; i++) {
        draw3DObject(
            wall_mesh.shape, wall_mesh.texture,
            Vec3f(0, 0, 2*(i+.5)),
            Vec3f(1, -1, 1)
        );
    }

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    draw3DObject(
        racket_mesh, player.racket.position + Vec3f(0, 0, 0),
        player.racket.scale
    );
    
    for(auto& box : level.obstacles) {
        PhysicsAABB tmp_box {
            Vec3f(box.position - box.size),
            Vec3f(box.position + box.size),
        };
        drawAABB(tmp_box);
    }
    for(auto& box : level.walls) {
        PhysicsAABB tmp_box {
            Vec3f(box.position - box.size),
            Vec3f(box.position + box.size),
        };
        drawAABB(tmp_box);
    }
}


int main(int argc, const char* argv[]) {
    Window win {800, 600, "fenetre"};
    
    Player player;
    player.spawn();

    Level level (20);

    level.obstacles.push_back(Obstacle {
        Vec3f(.5, 0, 5),
        Vec3f(.5, 1, .25)
    });
    level.obstacles.push_back(Obstacle {
        Vec3f(-.5, 0, 10),
        Vec3f(.5, 1, .25)
    });

    win.on_mouse_move = [&player, &win](double xpos, double ypos) {
        player.racket.position = Vec3f(clamp(xpos, -0.7, 0.7), clamp(ypos, -0.7, 0.7), 2);
        std::cout << "xpos: " << xpos << " ypos: " << ypos << std::endl;
        if (player.racket.hasBall) {
            player.ball.position = player.racket.position + Vec3f(0, 0, 1);
        }
    };
    win.on_mouse_button = [&player](int button, int action, int mods) {

    };

    double timer = 0;
    double deltaTime = 0;
    while(!win.shouldClose()) {
        double startTime = glfwGetTime();

        player.ball.move(deltaTime);
        player.ball.collide(level.walls);
        player.ball.collide(level.obstacles);

        win.clear();
        drawWorld(win, player, level);

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
