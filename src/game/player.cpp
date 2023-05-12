#include "main.h"
#include "IHM/draw.h"
#include "logic/physics.h"
#include "game/player.h"
#include "game/ball.h"

void Player::spawn() {
    this->ball.position = Vec3f(0, 0, 2);
    this->ball.speed_dir = Vec3f(0, 0, 0);
    this->ball.speed = 0;
    this->racket.position = Vec3f(0, 0, 0);
}

void Player::launchBall() {
    this->racket.hasBall = false;

}

void Player::receiveBall() {
    this->racket.hasBall = true;

}