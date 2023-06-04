#include "main.h"
#include "IHM/draw.h"
#include "logic/physics.h"
#include "game/player.h"
#include "game/ball.h"

Player::Player() {
    this->spawn();
}

void Player::spawn() {
    this->ball.position = Vec3f(0, 0, SPAWN_Z) + BALL_SHIFT;
    this->ball.speed_dir = Vec3f(0, 0, 0);
    this->ball.speed = 0;
    this->racket.position = Vec3f(0, 0, SPAWN_Z);
}

void Player::launchBall() {
    if(this->racket.hasBall) {
        this->racket.hasBall = false;
        this->ball.goToward(Vec3f(0, 0, 1), 3);
    }
}

void Player::receiveBall() {
    this->racket.hasBall = true;
    this->ball.speed_dir = Vec3f(0, 0, 0);
    this->ball.speed = 0;
    this->ball.position = this->racket.position + BALL_SHIFT;
    this->racket.isMovingForward = false;
    this->hasPickedBall = true;
}

void Player::setPosition(float x, float y) {
    Vec3f pos = Vec3f( x , y , this->racket.position.z );
    this->racket.setPosition(pos);
    if(this->racket.hasBall) {
        this->ball.position = pos + BALL_SHIFT;
    }
}

void Player::loseALife() {
    this->lives -= 1;
    this->hasLostALife = true;
    this->ball.speed = 0;
    this->isReady = false;
}

void Player::recover(double delta_time) {
    if(this->hasLostALife) {
        this->recoverTime = RECOVER_TIME_START;
        this->isReady = false;
        this->hasLostALife = false;
        return;
    }
    if(this->recoverTime <= 0 && !this->isReady) {
        this->receiveBall();
        this->isReady = true;
        return;
    }
    this->recoverTime -= delta_time;
}

void Player::makeAllCollisions(std::vector<Obstacle>& obstacles) {
    this->ball.has_collided_wall |= this->ball.collideAndBounce(obstacles);
    if(
        !this->racket.hasBall && 
        this->ball.isColliding(this->racket.hitbox)
    ) {
        if(this->racket.canCatchBall) {
            this->receiveBall();
            return;
        }
        this->racket.position + BALL_SHIFT;
        this->ball.goToward(
            this->ball.position - (this->racket.position - Vec3f(0,0,.5))
        );
        this->ball.has_collided_racket |= true;
    }
    if(this->ball.position.z < this->racket.position.z - 1) {
        this->loseALife();
    }
}

void Player::update(float delta_time) {
    this->ball.update(delta_time);
    this->racket.update(delta_time);
    this->hasPickedBall = false;
    this->hasPickedBonus = false;
}
