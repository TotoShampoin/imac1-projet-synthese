#include "game/ball.h"
#include "IHM/objects.h"

Mesh makeBallMesh() {
    return Mesh {
        .shape = createSphere(16),
        .texture = Image("assets/textures/ball.png")
    };
};

void Ball::goToward(Vec3f vec_speed) {
    speed_dir = vec_speed.normalize();
}
void Ball::goToward(Vec3f direction, float speed_value) {
    speed_dir = direction.normalize();
    speed = speed_value;
}

void Ball::move(float delta_time) {
    if(speed_dir.norm() > 0) {
        speed_dir = speed_dir.normalize();
    }
    position += speed_dir * speed * delta_time;
}

bool Ball::isColliding(Obstacle& box) {
    PhysicsSphere this_ball (position, radius);
    return this_ball.collide(box);
}
bool Ball::isColliding(std::vector<Obstacle>& boxes) {
    PhysicsSphere this_ball (position, radius);
    for(auto& box : boxes) {
        if(this_ball.collide(box)) return true;
    }
    return false;
}

void Ball::bounce(Vec3f& normal) {
    Vec3f normalized = normal.normalize();
    speed_dir = (speed_dir - normalized * 2 * (speed_dir*normalized)).normalize();
}

bool Ball::collideAndBounce(Obstacle& box) {
    PhysicsSphere this_ball (position, radius);
    Vec3f normal (0,0,0);

    bool collides = this_ball.collide(box, normal);

    if(!collides) {
        return false;
    }

    normal = normal.normalize();
    speed_dir = (speed_dir - normal * 2 * (speed_dir*normal)).normalize();
    return true;
}

bool Ball::collideAndBounce(std::vector<Obstacle>& boxes) {
    PhysicsSphere this_ball (position, radius);
    Vec3f normal (0,0,0);
    bool collides = false;

    for(auto& box : boxes) {
        Vec3f tmp_normal (0,0,0);
        bool collided_this = this_ball.collide(box, tmp_normal);
        collides |= collided_this;
        normal += tmp_normal;
    }

    if(!collides) {
        return false;
    }
    
    normal = normal.normalize();
    speed_dir = (speed_dir - normal * 2 * (speed_dir*normal)).normalize();
    return true;
}


void Ball::update(float delta_time) {
    move(delta_time);
    if(has_collided) {
        collision_counter ++;
    } else {
        collision_counter = 0;
    }
    has_collided = false;
    has_collided_wall = false;
    has_collided_obstacle = false;
    has_collided_racket = false;
}
