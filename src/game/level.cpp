#include "game/level.h"
#include "IHM/objects.h"

Level::Level() {
    obstacles.push_back(Obstacle {
        Vec3f(0, 1.25, length/2),
        Vec3f(1, .25, length/2)
    });
    obstacles.push_back(Obstacle {
        Vec3f(1.25, 0, length/2),
        Vec3f(.25, 1, length/2)
    });
    obstacles.push_back(Obstacle {
        Vec3f(-1.25, 0, length/2),
        Vec3f(.25, 1, length/2)
    });
    obstacles.push_back(Obstacle {
        Vec3f(0, -1.25, length/2),
        Vec3f(1, .25, length/2)
    });
}

Mesh makeWallMesh() {
    return {
        .shape = createWall(),
        .texture = Image("assets/walls.png"),
    };
}
