#include "main.h"
#include "IHM/draw.h"
#include "logic/physics.h"
#include "game/player.h"
#include "game/ball.h"

void Player::spawn() {
    this->ball.position = Vec3f(0, 0, 2);
    this->ball.speed_dir = Vec3f(0, 0, 0);
    this->ball.speed = 0;
    this->racket.position = Vec3f(0, 0, 2);
}

void Player::launchBall() {
    this->racket.hasBall = false;
    this->ball.goToward(Vec3f(0, 0, 1), 3);
}

void Player::receiveBall() {
    this->racket.hasBall = true;
    this->ball.speed_dir = Vec3f(0, 0, 0);
    this->ball.speed = 0;
}

void Player::setPosition(float x, float y) {
    Vec3f pos = Vec3f( x , y , this->racket.position.z );
    this->racket.setPosition(pos);
    if(this->racket.hasBall) {
        this->ball.position = pos + BALL_SHIFT;
    }
}

void Player::makeAllCollisions(std::vector<Obstacle>& obstacles) {
    this->ball.collideAndBounce(obstacles);
    if(!this->racket.hasBall && this->ball.isColliding(this->racket.hitbox)) {
        this->racket.position + BALL_SHIFT;
        this->ball.goToward(
            this->ball.position - (this->racket.position - Vec3f(0,0,.5))
        );
    }
}

void Player::update(float delta_time) {
    this->ball.update(delta_time);
    this->racket.update(delta_time);
}
