#include "main.h"
#include "IHM/draw.h"
#include "logic/physics.h"

#include "game/player.h"
#include "game/level.h"

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
            Vec3f(LEVEL_WIDTH, -LEVEL_HEIGHT, 1)
        );
    }

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    // draw3DObject(
    //     racket_mesh, player.racket.position + Vec3f(0, 0, 0),
    //     player.racket.scale
    // );
    drawAABB(player.racket.hitbox);
    
    for(auto& box : level.obstacles) {
        drawAABB(box);
    }
    for(auto& box : level.walls) {
        drawAABB(box);
    }
}


int main(int argc, const char* argv[]) {
    Window win {800, 600, "fenetre"};
    
    Player player;
    player.spawn();

    Level level (20);

    level.obstacles.push_back(Obstacle {
        Vec3f(0, -LEVEL_HEIGHT, 5),
        Vec3f(LEVEL_WIDTH, LEVEL_HEIGHT, 5.5)
    });
    level.obstacles.push_back(Obstacle {
        Vec3f(-LEVEL_WIDTH, -LEVEL_HEIGHT, 10),
        Vec3f(0, LEVEL_HEIGHT, 10.5)
    });

    win.on_mouse_move = [&player, &win](double xpos, double ypos) {
        player.racket.position = Vec3f(
            clamp(xpos, -(LEVEL_WIDTH - 0.3), LEVEL_WIDTH - 0.3), 
            clamp(ypos, -(LEVEL_HEIGHT - 0.3), LEVEL_HEIGHT - 0.3), 
            2
        );
        player.racket.hitbox.boundA = Vec3f(-1 * player.racket.scale.x,
                                        -1 * player.racket.scale.y,
                                        0.0) + player.racket.position;
        player.racket.hitbox.boundB = Vec3f(1 * player.racket.scale.x,
                                        1 * player.racket.scale.y,
                                        -0.5) + player.racket.position;
        //std::cout << "xpos: " << xpos << " ypos: " << ypos << std::endl;
        //std::cout << "collider_xpos: " << player.racket.hitbox.middle().x << " collider_ypos: " << player.racket.hitbox.middle().y << std::endl;
        std::cout << "boundA: " << player.racket.hitbox.boundA.x << " " <<
        player.racket.hitbox.boundA.y << " " <<
        player.racket.hitbox.boundA.z << " boundB: " <<
        player.racket.hitbox.boundB.x << " " <<
        player.racket.hitbox.boundB.y << " " <<
        player.racket.hitbox.boundB.z << std::endl;
        std::cout << "ball_position: " << player.ball.position.x << " " <<
        player.ball.position.y << " " <<
        player.ball.position.z << std::endl;
        if (player.racket.hasBall) {
            player.ball.position = player.racket.position + Vec3f(0, 0, 1);
        }
    };
    win.on_mouse_button = [&player](int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            player.racket.hasBall = false;
            player.ball.speed = 3;
            //player.ball.speed_dir = Vec3f(-player.racket.position.x, -player.racket.position.y, 1);
            player.ball.speed_dir = Vec3f(0, 0, 1);
        }
    };

    double timer = 0;
    double deltaTime = 0;
    while(!win.shouldClose()) {
        double startTime = glfwGetTime();

        player.ball.move(deltaTime);
        player.ball.collide(level.walls);
        player.ball.collide(level.obstacles);
        player.ball.collide(player.racket.hitbox);

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
