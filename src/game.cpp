#include "logic/physics.h"
#include "IHM/objects.h"
#include "IHM/draw.h"
#include "IHM/audio.h"
#include "game/player.h"
#include "game/level.h"
#include "game.h"
#include "draw-game.h"

struct Game {
    Player player;
    Level level;
    Window& window;
    bool is_pause = false;
    bool pause_started = false;
    bool pause_ended = false;
    bool quits = false;
    double timer = 0;
    int score = 0;

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
                case GLFW_KEY_A: {
                    game.quits = true;
                } break;
                }
            }
        };
    }
    void makeVictoryEvents(bool& confirm) {
        Game& game = *this;
        Window& win = window;

        win.on_mouse_move = [](double xpos, double ypos) {};
        win.on_mouse_button = [&confirm](int button, int action, int mods) {
            if(action == GLFW_RELEASE) {
                confirm = true;
            }
        };
        win.on_key = [&confirm](int key, int scancode, int action, int mods) {
            if(action == GLFW_RELEASE) {
                confirm = true;
            }
        };
    }
    void makeNoEvents() {
        Game& game = *this;
        Window& win = window;

        win.on_mouse_move = [](double xpos, double ypos) {};
        win.on_mouse_button = [](int button, int action, int mods) {};
        win.on_key = [](int key, int scancode, int action, int mods) {};
    }

    void pause() {
        is_pause = true;
        pause_started = true;
        makePauseEvents();
    }
    void resume() {
        is_pause = false;
        pause_ended = true;
        makeGameEvents();
    }
    void update(double delta_time) {
        pause_started = false;
        pause_ended = false;
        if(!is_pause) {
            timer += delta_time;
        }
    }
};


void physics(Game& game, double delta_time) {
    Level& level = game.level;
    Player& player = game.player;
    // std::vector<Obstacle> obstacles = level.getAllObstacles();

    player.update(delta_time);
    player.makeRacketCollisions(level.obstacles, Vec2f(level.width, level.height));
    player.reposition();
    player.makeBallCollisions(level.obstacles, level.walls);

    if(player.ball.collision_counter > 30) {
        player.ball.position.x = 0;
        player.ball.position.y = 0;
        player.ball.position.z -= .5;
        while(player.ball.isColliding(level.obstacles) || player.ball.isColliding(level.walls)) {
            player.ball.position.z -= .25;
        }
        if(player.ball.position.z < player.racket.position.z) {
            player.racket.position.z -= 2;
        }
    }

    for(auto& bonus : level.bonus) {
        PhysicsAABB hitbox = bonus.getHitbox();
        bonus.update(delta_time);
        if(!bonus.is_picked && player.ball.isColliding(hitbox)) {
            bonus.bePicked(player);
            player.bonus_picked++;
            player.hasPickedBonus = true;
        }
    }
}

void display(Window& win, Game& game, double delta_time) {
    static Font font ("assets/textures/fonts/minecraft.png", 16, 16, 8, 16, '\0', 16, 16);
    font.texture.setFilter(GL_NEAREST,GL_NEAREST);

    char score_string[16];
    char life_string[16];

    sprintf(score_string, "SCR: %10d", game.score);
    sprintf(life_string, "LIF: %10d", game.player.lives);

    Level& level = game.level;
    Player& player = game.player;

    win.clear();

    use3dMode(win);
    
    placeCamera(player, level);
    
    drawBall(player.ball);
    drawRacket(player.racket);
    drawLevel(level, player, game.timer);

    //

    use2dMode(win);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    glColor4f(0, 0, 0, .5);
    draw2DBox(Vec2f(win.aspect_ratio - 15*16/600., -1 + 64/600.), Vec2f(15*16/600., 64/600.));
    glColor4f(1, 1, 1, 1);
    draw2DText(life_string, font, Coord2D {Vec2f(win.aspect_ratio - 15*16/600., -1 + 32/600.), Vec2f(32/1200., 32/1200.), 0});
    draw2DText(score_string, font, Coord2D {Vec2f(win.aspect_ratio - 15*16/600., -1 + 96/600.), Vec2f(32/1200., 32/1200.), 0});
    
    if(game.is_pause) {
        glColor4f(0, 0, 0, .5);
        draw2DBox(Vec2f(0, 0), Vec2f(win.aspect_ratio, 1));
        glColor4f(1, 1, 1, 1);
        draw2DText("Pause", font, Vec2f(0, -192/600.), Vec2f(128/1200., 128/1200.));
        draw2DText("[ESC] resume - [Q] quit", font, Vec2f(0, 64/600.), Vec2f(32/1200., 32/1200.));
    }
    
    win.refresh();
}

// TODO: Un son différent pour quand ça rebondit et quand on grab la balle
void audio(Game& game, double delta_time) {
    static AudioMedia bounce_obstacle ("assets/sounds/bounce_enemy.wav");
    static AudioMedia bounce_racket ("assets/sounds/bounce_racket.wav");
    static AudioMedia bounce_wall ("assets/sounds/bounce_wall.wav");
    static AudioMedia bonus ("assets/sounds/bonus.wav");
    static AudioMedia damage ("assets/sounds/damage.wav");
    static AudioMedia pause ("assets/sounds/pause.wav");
    static AudioMedia racket_grab ("assets/sounds/racket_grab.wav");
    static AudioMedia resume ("assets/sounds/resume.wav");

    if(game.pause_started) {
        pause.stop();
        pause.play();
    }
    if(game.pause_ended) {
        resume.stop();
        resume.play();
    }

    if(game.is_pause) {
        return;
    }

    Level& level = game.level;
    Player& player = game.player;

    if(player.ball.has_collided_wall) {
        bounce_wall.stop();
        bounce_wall.play();
    }
    if(player.ball.has_collided_obstacle) {
        bounce_obstacle.stop();
        bounce_obstacle.play();
    }
    if(player.ball.has_collided_racket) {
        bounce_racket.stop();
        bounce_racket.play();
    }
    if(player.hasPickedBonus) {
        bonus.stop();
        bonus.play();
    }
    if(player.hasPickedBall) {
        racket_grab.stop();
        racket_grab.play();
    }
    if(player.hasLostALife) {
        damage.stop();
        damage.play();
    }
}


void startGameLoop(Window& win, Game& game) {
    double delta_time = 0;

    Level& level = game.level;
    Player& player = game.player;
    
    while(!(win.shouldClose() || player.hasReachedEndLine || (player.lives <= 0 && player.isReady) || game.quits)) {
        win.pollEvents();

        game.score = (player.racket.position.z - SPAWN_Z) * 100 + (player.bonus_picked * 300);
        
        player.recover(delta_time);
        if(!game.is_pause && game.player.isReady) {
            physics(game, delta_time);
        }
        display(win, game, delta_time);
        audio(game, delta_time);

        game.update(delta_time);

        delta_time = timer();
    }
}

void endSequence(Window& win, Game& game) {
    static AudioMedia game_over ("assets/sounds/game_over.wav");
    static AudioMedia victory ("assets/sounds/victory.wav");
    static Font font ("assets/textures/fonts/minecraft.png", 16, 16, 8, 16, '\0', 16, 16);
    font.texture.setFilter(GL_NEAREST,GL_NEAREST);

    double delta_time = 0;
    double time = 0;

    Level& level = game.level;
    Player& player = game.player;
    game.is_pause = false;

    bool can_confirm = false;
    bool confirm = false;

    char message[10];
    char score_string[12];
    sprintf(score_string, "%10d", game.score);

    if(player.hasReachedEndLine) {
        sprintf(message, "Victory!");
        victory.stop();
        victory.play();
    } else {
        sprintf(message, "Game Over");
        game_over.stop();
        game_over.play();
    }
    
    game.makeNoEvents();

    while(!(win.shouldClose() || confirm)) {
        if(!can_confirm && time > 3) {
            game.makeVictoryEvents(confirm);
            can_confirm = true;
        }

        win.pollEvents();

        win.clear();
        use3dMode(win);
        
        placeCamera(player, level);
        drawBall(player.ball);
        drawRacket(player.racket);
        drawLevel(level, player, game.timer);

        use2dMode(win);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

        glColor4f(0, 0, 0, remap(time, 0, 2, 0, 1));
        draw2DBox(Vec2f(0, 0), Vec2f(win.aspect_ratio, 1));
        glColor4f(1, 1, 1, 1);
        draw2DText(message, font, Coord2D {
            Vec2f(0, remap(time, .25, 1, -1.25, -.5)),
            Vec2f(96/1200., 96/1200.)
        });
        draw2DText(score_string, font, Coord2D {
            Vec2f(0, remap(time, .5, 1.25, 1.25, .5)),
            Vec2f(64/1200., 64/1200.)
        });
        if(can_confirm) {
            draw2DText("Press a key to continue", font, Coord2D {
                Vec2f(0, 0),
                Vec2f(32/1200., 32/1200.)
            });
        }

        win.refresh();

        delta_time = timer();
        time += delta_time;
    }
}


void main_game(const char* level_path, Window& win) {
    initTypeTable();

    Game game {
        Player(),
        Level(level_path),
        win
    };

    startGameLoop(win, game);
    if(!game.quits) endSequence(win, game);
}

