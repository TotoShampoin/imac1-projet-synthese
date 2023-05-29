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
    has_collided = true;
    return true;
}

bool Ball::collideAndBounce(std::vector<Obstacle>& boxes) {
    PhysicsSphere this_ball (position, radius);
    Vec3f normal (0,0,0);
    bool collides = false;

    for(auto& box : boxes) {
        Vec3f tmp_normal (0,0,0);
        collides |= this_ball.collide(box, tmp_normal);
        normal += tmp_normal;
    }

    if(!collides) {
        return false;
    }
    
    normal = normal.normalize();
    speed_dir = (speed_dir - normal * 2 * (speed_dir*normal)).normalize();
    has_collided = true;
    return true;
}


void Ball::update(float delta_time) {
    move(delta_time);
    has_collided = false;
}
