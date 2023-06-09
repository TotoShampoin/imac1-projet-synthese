#pragma once
#include "logic/vec3.h"
#include "IHM/objects.h"
#include "logic/physics.h"

#include <vector>

static const float BALL_SPEED = 4.;

struct Ball {
    Vec3f position = Vec3f(0,0,0);
    Vec3f speed_dir = Vec3f(0,0,0);
    float speed = BALL_SPEED;
    float radius = 0.25;
    bool has_collided = false;
    bool has_collided_wall = false;
    bool has_collided_obstacle = false;
    bool has_collided_racket = false;
    int collision_counter = 0;

    void goToward(Vec3f vec_speed);
    void goToward(Vec3f direction, float speed_value);

    void move(float delta_time);

    bool isColliding(Obstacle&);
    bool isColliding(std::vector<Obstacle>& boxes);

    void bounce(Vec3f& normal);

    bool collideAndBounce(Obstacle&);
    bool collideAndBounce(std::vector<Obstacle>&);

    void update(float delta_time);
};

Mesh makeBallMesh();

