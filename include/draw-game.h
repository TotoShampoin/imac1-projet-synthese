#include "game/player.h"
#include "game/level.h"
#include "IHM/draw.h"
#include "common.h"

void placeCamera(Player& player, Level& level);

void drawBall(Ball& ball);
void drawRacket(Racket& racket);
void drawLevel(Level& level, Player& player, double time);
