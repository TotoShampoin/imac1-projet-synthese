#include "main.h"
#include "IHM/draw.h"
#include "logic/physics.h"
#include "game/player.h"
#include "game/ball.h"

void Player::spawn() {
    static Mesh ball_mesh = makeBallMesh();
    //Mesh racket_mesh = makeRacketMesh();
    
}