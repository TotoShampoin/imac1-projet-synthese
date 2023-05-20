#include "game/level.h"
#include "IHM/objects.h"

Level::Level(float l) {
    length = l;
    walls.push_back(Obstacle {
        Vec3f(0, 0, -.5),
        Vec3f(LEVEL_WIDTH, LEVEL_HEIGHT, -.5)
    });
    walls.push_back(Obstacle {
        Vec3f(0, 0, length+.5),
        Vec3f(LEVEL_WIDTH, LEVEL_HEIGHT, -.5)
    });
    walls.push_back(Obstacle {
        Vec3f(0, LEVEL_HEIGHT+.25, length/2),
        Vec3f(LEVEL_WIDTH, .25, length/2)
    });
    walls.push_back(Obstacle {
        Vec3f(LEVEL_WIDTH+.25, 0, length/2),
        Vec3f(.25, LEVEL_HEIGHT, length/2)
    });
    walls.push_back(Obstacle {
        Vec3f(-(LEVEL_WIDTH+.25), 0, length/2),
        Vec3f(.25, LEVEL_HEIGHT, length/2)
    });
    walls.push_back(Obstacle {
        Vec3f(0, -(LEVEL_HEIGHT+.25), length/2),
        Vec3f(LEVEL_WIDTH, .25, length/2)
    });
}

Mesh makeWallMesh() {
    return {
        .shape = createWall(),
        .texture = Image("assets/textures/walls.png"),
    };
}
