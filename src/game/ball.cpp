#include "game/ball.h"
#include "IHM/objects.h"

Mesh makeBallMesh() {
    return Mesh {
        .shape = createSphere(16),
        .texture = Image("assets/textures/ball.png")
    };
};


void Ball::move(float deltaTime) {
    if(speed_dir.norm() > 0) {
        speed_dir = speed_dir.normalize();
    }
    position += speed_dir * speed * deltaTime;
}

bool Ball::collide(Obstacle& box) {
    PhysicsSphere this_ball (position, radius);
    Vec3f normal (0,0,0);
    bool collides = this_ball.collide(box, normal);
    if(!collides) return false;
    normal = normal.normalize();
    speed_dir = (speed_dir - normal * 2 * (speed_dir*normal)).normalize();
    return true;
}

bool Ball::collide(std::vector<Obstacle>& boxes) {
    PhysicsSphere this_ball (position, radius);
    Vec3f normal (0,0,0);
    bool collides = false;
    for(auto& box : boxes) {
        Vec3f tmp_normal (0,0,0);
        collides |= this_ball.collide(box, tmp_normal);
        normal += tmp_normal;
    }
    if(!collides) return false;
    normal = normal.normalize();

    speed_dir = (speed_dir - normal * 2 * (speed_dir*normal)).normalize();
    return true;
}

