#include "main.h"
#include "common.h"
#include "game.h"

#include "IHM/draw.h"
#include "IHM/audio.h"
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
        0, 0, player.racket.position.z - 2,
        0, 0, level.length,
        0, 1, 0
    );
    
    draw3DObject(
        ball_mesh.shape, ball_mesh.texture, 
        player.ball.position + Vec3f(0,0,0),
        Vec3f(1,1,1) * player.ball.radius
    );
    int size = level.length/2;
    for (int i = 0; i < size; i++) {
        float wall_distance = 2*(i+.5);
        float distance_to_player = abs(player.racket.position.z - wall_distance);
        float wall_light = 1 - distance_to_player / 10;
        std::cout << wall_light << std::endl;
        for(int j = 0; j < 16; j++) {
            set_coord(wall_mesh.shape.colors, j, wall_light, wall_light, wall_light, 1);
        }
        draw3DObject(
            wall_mesh.shape, wall_mesh.texture,
            Vec3f(0, 0, wall_distance),
            Vec3f(LEVEL_WIDTH, -LEVEL_HEIGHT, 1)
        );
    }

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    drawAABB(player.racket.hitbox);
    
    for(auto& box : level.obstacles) {
        drawAABB(box);
    }
    for(auto& box : level.walls) {
        // drawAABB(box);
    }
}


int main(int argc, const char* argv[]) {
    Window win {800, 600, "fenetre"};

    // TODO: L'écran titre et l'écran de menu

    startGame("", win);
    
    Player player;
    player.spawn();

    Level level (20);

    // This is a test
    AudioContext channel;
    AudioMedia tac ("assets/sounds/switch.wav");

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
            player.racket.position.z
        );
        player.racket.hitbox.boundA = Vec3f(-1 * player.racket.scale.x,
                                        -1 * player.racket.scale.y,
                                        0.0) + player.racket.position;
        player.racket.hitbox.boundB = Vec3f(1 * player.racket.scale.x,
                                        1 * player.racket.scale.y,
                                        -0.1) + player.racket.position;
        if (player.racket.hasBall) {
            player.ball.position = player.racket.position + Vec3f(0, 0, 1);
        }
    };
    win.on_mouse_button = [&player](int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            player.racket.hasBall = false;
            player.ball.speed = 3;
            player.ball.speed_dir = Vec3f(0, 0, 1);
        }
    };
    win.on_key = [&player](int key, int scancode, int action, int mods) {
        player.racket.isMovingForward = !player.racket.hasBall && key == GLFW_KEY_W;
    };

    double timer = 0;
    double deltaTime = 0;
    while(!win.shouldClose()) {
        double startTime = glfwGetTime();
        int keyState_W = win.getKey(GLFW_KEY_W);

        player.ball.move(deltaTime);

        bool collided = false;
        collided |= player.ball.collide(level.walls);
        collided |= player.ball.collide(level.obstacles);
        collided |= player.ball.collide(player.racket.hitbox);
        if(collided) {
            tac.stop();
            tac.play();
        }

        if (player.racket.isMovingForward && keyState_W == GLFW_PRESS) {
            player.racket.position.z += 0.01;
            player.racket.hitbox.boundA = Vec3f(-1 * player.racket.scale.x,
                                                -1 * player.racket.scale.y,
                                                0.0) + player.racket.position;
            player.racket.hitbox.boundB = Vec3f(1 * player.racket.scale.x,
                                                1 * player.racket.scale.y,
                                                -0.1) + player.racket.position;
        } else {
            player.racket.isMovingForward = false;
        }

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