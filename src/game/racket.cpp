#include "game/player.h"
#include "game/ball.h"
#include "IHM/objects.h"

void Racket::updateHitbox() {
    this->hitbox.boundA = Vec3f(
        -1 * this->scale.x,
        -1 * this->scale.y,
        0.0
    ) + this->position;
    this->hitbox.boundB = Vec3f(
        1 * this->scale.x,
        1 * this->scale.y,
        -0.1
    ) + this->position;
}

void Racket::setPosition(Vec3f pos) {
    this->position = pos;
    this->updateHitbox();
}

void Racket::move(float delta_time) {
    if(this->isMovingForward) {
        this->position.z += delta_time;
        this->updateHitbox();
    }
}

void Racket::update(float delta_time) {
    this->move(delta_time);
    this->updateHitbox();
}

Geometry makeRacketMesh() {
    return createCube();
};
