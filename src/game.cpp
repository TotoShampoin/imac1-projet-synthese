#include "game.h"
#include "IHM/objects.h"
#include "IHM/draw.h"
#include "IHM/audio.h"
#include "logic/physics.h"

#include "game/player.h"
#include "game/level.h"


struct Game {
    Player player;
    Level level;
    Window& window;
    bool is_pause = false;
    bool pause_switched = false;
    double timer = 0;

    Game(Player pl, Level lv, Window& wn): 
        player(pl),
        level(lv),
        window(wn)
    {
        makeGameEvents();
    }

    void makeGameEvents() {
        Game& game = *this;
        Level& level = this->level;
        Player& player = this->player;
        Window& win = window;

        win.on_mouse_move = [&player, &win, &level](double xpos, double ypos) {
            float lim_x = level.width - player.racket.scale.x;
            float lim_y = level.height - player.racket.scale.y;
            player.setPosition(
                clamp(xpos, -lim_x, lim_x), 
                clamp(ypos, -lim_y, lim_y)
            );
        };
        win.on_mouse_button = [&player](int button, int action, int mods) {
            if(action == GLFW_PRESS) {
                switch(button) {
                case GLFW_MOUSE_BUTTON_LEFT: {
                    player.launchBall();
                } break;
                }
            }
        };
        win.on_key = [&player, &game](int key, int scancode, int action, int mods) {
            if(action == GLFW_PRESS) {
                switch(key) {
                case GLFW_KEY_W: {
                    player.racket.isMovingForward = !player.racket.hasBall;
                } break;
                case GLFW_KEY_ESCAPE: {
                    game.pause();
                } break;
                }
            }
            if(action == GLFW_RELEASE) {
                switch(key) {
                case GLFW_KEY_W: {
                    player.racket.isMovingForward = false;
                } break;
                }
            }
        };
    }
    void makePauseEvents() {
        Game& game = *this;
        Window& win = window;

        win.on_mouse_move = [](double xpos, double ypos) {};
        win.on_mouse_button = [](int button, int action, int mods) {};
        win.on_key = [&game](int key, int scancode, int action, int mods) {
            if(action == GLFW_PRESS) {
                switch(key) {
                case GLFW_KEY_ESCAPE: {
                    game.resume();
                } break;
                }
            }
        };
    }

    void pause() {
        is_pause = true;
        pause_switched = true;
        makePauseEvents();
    }
    void resume() {
        is_pause = false;
        pause_switched = true;
        makeGameEvents();
    }
    void update(double delta_time) {
        pause_switched = false;
        if(!is_pause) {
            timer += delta_time;
        }
    }
};


void physics(Game& game, double delta_time) {
    Level& level = game.level;
    Player& player = game.player;
    std::vector<Obstacle> obstacles = level.getAllObstacles();

    player.update(delta_time);
    player.makeAllCollisions(obstacles);

    for(auto& bonus : level.bonus) {
        PhysicsAABB hitbox = bonus.getHitbox();
        bonus.update(delta_time);
        if(!bonus.is_picked && player.ball.isColliding(hitbox)) {
            bonus.bePicked(player);
        }
    }
}

void display(Window& win, Game& game, double delta_time) {
    static Mesh ball_mesh = makeBallMesh();
    static Mesh wall_mesh = makeWallMesh();
    static Geometry racket_mesh = makeRacketMesh();
    static Geometry bonus_mesh = createSphere(4);

    Level& level = game.level;
    Player& player = game.player;

    win.clear();

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
        // std::cout << wall_light << std::endl;
        for(int j = 0; j < 16; j++) {
            set_coord(wall_mesh.shape.colors, j, wall_light, wall_light, wall_light, 1);
        }
        draw3DObject(
            wall_mesh.shape, wall_mesh.texture,
            Vec3f(0, 0, wall_distance),
            Vec3f(level.width, -level.height, 1)
        );
    }

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    drawAABB(player.racket.hitbox);
    
    for(auto& box : level.obstacles) {
        drawAABB(box);
    }
    for(auto& bonus : level.bonus) {
        PhysicsAABB hitbox = bonus.getHitbox();
        drawAABB(hitbox);
        if(bonus.specs->is_victory || bonus.is_picked) {
            continue;
        }
        draw3DObject(bonus_mesh, bonus.position, bonus.specs->size / 2, Vec3f(0, 1, 1), game.timer * 2 * 180 / M_PI);
    }

    //

    use2dMode(win);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    if(game.is_pause) {
        glColor4f(0, 0, 0, .5);
        draw2DBox(Vec2f(0, 0), Vec2f(win.aspect_ratio, 1));
    }
    
    win.refresh();
}

// TODO: Un son différent pour quand ça rebondit et quand on grab la balle
void audio(Game& game, double delta_time) {
    static AudioContext channel;
    static AudioMedia tac ("assets/sounds/switch.wav");

    if(game.is_pause) {
        return;
    }

    Level& level = game.level;
    Player& player = game.player;

    if(player.ball.has_collided) {
        tac.stop();
        tac.play();
    }
}


void gameLoop(Window& win, Game& game) {
    static double delta_time = 0;

    Level& level = game.level;
    Player& player = game.player;
    
    win.pollEvents();
    if(!game.is_pause) {
        physics(game, delta_time);
    }
    display(win, game, delta_time);
    audio(game, delta_time);

    game.update(delta_time);

    delta_time = timer();
}


void startGame(const char* level_path, Window& win) {
    initTypeTable();

    Game game {
        Player(),
        Level(level_path),
        win
    };

    while(!win.shouldClose()) {
        gameLoop(win, game);
    }

}

