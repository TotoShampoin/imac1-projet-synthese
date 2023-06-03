#include "common.h"

#include <cstring>

float clamp(float t, float min, float max) {
    return (t < min) ? min : (t > max) ? max : t;
}

float lerp(float t, float a, float b) {
    return (1 - t) * a + t * b;
}

float invlerp(float v, float a, float b) {
    return ( v - a ) / ( b - a );
}

float remap(float t, float t_start, float t_end, float a, float b) {
    float ct = clamp(t, t_start, t_end);
    float it = invlerp(ct, t_start, t_end);
    return lerp(it, a, b);
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

bool endsWith(const char* str,  const char* endstr) {
    if(!str || !endstr) return false;
    size_t len_str = strlen(str);
    size_t len_end = strlen(endstr);
    if(len_end > len_str) return false;
    return strncmp(str + len_str - len_end, endstr, len_end) == 0;
}
