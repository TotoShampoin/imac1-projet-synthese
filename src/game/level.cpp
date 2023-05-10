#include "game/level.h"
#include "IHM/objects.h"

Mesh makeWallMesh() {
    return {
        .shape = createWall(),
        .texture = Image("assets/walls.png"),
    };
}
