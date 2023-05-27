#include "common.h"

float clamp(float t, float min, float max) {
    return (t < min) ? min : (t > max) ? max : t;
}

void drawAABB(PhysicsAABB& box) {
    static Geometry geo = createCube();
    Vec3f center = box.middle();
    Vec3f size = box.size();
    draw3DObject(geo, center, size);
}

double timer() {
    static double start_time = 0;
    double delta_time = glfwGetTime() - start_time;
    while(delta_time < FRAME_TIME) {
        delta_time = glfwGetTime() - start_time;
    }
    start_time = glfwGetTime();
    return delta_time;
}
