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
    bool pause_switched = false;
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
            player.bonus_picked++;
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
        draw2DText("Pause", font, Vec2f(0, -.5), Vec2f(.25, .25));
    }
    
    win.refresh();
}

// TODO: Un son différent pour quand ça rebondit et quand on grab la balle
void audio(Game& game, double delta_time) {
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


void startGameLoop(Window& win, Game& game) {
    double delta_time = 0;

    Level& level = game.level;
    Player& player = game.player;
    
    while(!(win.shouldClose() || player.hasReachedEndLine || (player.lives <= 0 && player.isReady))) {
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

void endSequence(Window& win, Game& game, const char* message) {
    static Font font ("assets/textures/fonts/minecraft.png", 16, 16, 8, 16, '\0', 16, 16);
    font.texture.setFilter(GL_NEAREST,GL_NEAREST);

    double delta_time = 0;
    double time = 0;

    Level& level = game.level;
    Player& player = game.player;
    game.is_pause = false;

    bool can_confirm = false;
    bool confirm = false;

    char score_string[12];
    sprintf(score_string, "%10d", game.score);
    
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
            Vec2f(80/1200., 80/1200.)
        });
        draw2DText(score_string, font, Coord2D {
            Vec2f(0, remap(time, .5, 1.25, 1.25, .5)),
            Vec2f(48/1200., 48/1200.)
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
    if(game.player.hasReachedEndLine) {
        endSequence(win, game, "Victory!");
    } else {
        endSequence(win, game, "Game over");
    }

}

