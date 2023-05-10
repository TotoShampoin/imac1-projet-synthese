#include "game/ball.h"
#include "IHM/objects.h"

Mesh makeBallMesh() {
    return Mesh {
        .shape = createSphere(16),
        .texture = Image("assets/tennis.png")
    };
};


void Ball::move(float deltaTime) {
    position += speed * deltaTime;
}

bool Ball::collide(std::vector<Obstacle>& boxes) {
    PhysicsSphere this_ball (position, radius);
    Vec3f normal (0,0,0);
    bool collides = false;
    for(auto& box : boxes) {
        Vec3f tmp_normal (0,0,0);
        PhysicsAABB tmp_box {
            Vec3f(box.position - box.size),
            Vec3f(box.position + box.size),
        };
        collides |= this_ball.collide(tmp_box, tmp_normal);
        normal += tmp_normal;
    }
    if(!collides) return false;
    normal.normalize();

    speed = speed - normal * 2 * (speed*normal);
    return true;
}

