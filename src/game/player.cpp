#include "main.h"
#include "IHM/draw.h"
#include "logic/physics.h"
#include "game/player.h"
#include "game/ball.h"

#include <cmath>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

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
        this->ball.goToward(Vec3f(0, 0, 1), BALL_SPEED);
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

void Player::reposition() {
    if(this->racket.position.z < SPAWN_Z) {
        this->racket.position.z = SPAWN_Z;
    }
    if(this->racket.hasBall) {
        this->ball.position = this->racket.position + BALL_SHIFT;
    }
}

void Player::setPosition(float x, float y) {
    this->nextPosition = Vec2f( x , y );
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
        this->racket.position.z -= 2;
        return;
    }
    this->recoverTime -= delta_time;
}

// This is very crude
void Player::makeRacketCollisions(std::vector<Obstacle>& obstacles, Vec2f limit) {
    PhysicsAABB racket_bigbox = this->racket.hitbox;
    if(this->racket.hasBall) {
        racket_bigbox.boundA.z += 1 + this->ball.radius;
    }

    Vec3f pos = this->racket.position;
    Vec3f size = this->racket.scale;

    for(auto obs : obstacles) {
        if(racket_bigbox.collide(obs)) {
            float lef = obs.boundMin().x - pos.x;
            float bot = obs.boundMin().y - pos.y;
            float rig = obs.boundMax().x - pos.x;
            float top = obs.boundMax().y - pos.y;
            float min_x = obs.boundMin().x - size.x;
            float max_x = obs.boundMax().x + size.x;
            float min_y = obs.boundMin().y - size.y;
            float max_y = obs.boundMax().y + size.y;
            if(min(fabs(rig), fabs(lef)) < min(fabs(top), fabs(bot))) {
                if(fabs(rig) > fabs(lef)) {
                    if(min_x - size.x > -limit.x/2) {
                        pos.x = min_x;
                    } else {
                        pos.x = max_x;
                    }
                } else {
                    if(max_x + size.x < limit.x/2) {
                        pos.x = max_x;
                    } else {
                        pos.x = min_x;
                    }
                }
            } else {
                if(fabs(top) > fabs(bot)) {
                    if(min_y - size.y > -limit.y/2) {
                        pos.y = max_y; // ?????
                    } else {
                        pos.y = min_y; // ?????
                    }
                } else {
                    if(max_y + size.y < limit.y/2) {
                        pos.y = min_y; // ?????
                    } else {
                        pos.y = max_y; // ?????
                    }
                }
            }
        }
    }
    this->racket.setPosition(pos);
}

void Player::makeBallCollisions(std::vector<Obstacle>& obstacles, std::vector<Obstacle>& walls) {
    this->ball.has_collided_wall |= this->ball.collideAndBounce(walls);
    this->ball.has_collided_obstacle |= this->ball.collideAndBounce(obstacles);
    this->ball.has_collided = this->ball.has_collided_wall || this->ball.has_collided_obstacle;
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
    this->racket.setPosition(this->nextPosition);
    this->ball.update(delta_time);
    this->racket.update(delta_time);

    this->reposition();
    this->hasPickedBall = false;
    this->hasPickedBonus = false;
}
