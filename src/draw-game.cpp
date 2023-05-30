#include "game/player.h"
#include "game/level.h"
#include "IHM/draw.h"
#include "common.h"

void placeCamera(Player& player, Level& level) {
    gluLookAt(
        0, 0, player.racket.position.z - 2,
        0, 0, level.length,
        0, 1, 0
    );
}

void drawBall(Ball& ball) {
    static Mesh ball_mesh = makeBallMesh();

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    draw3DObject(
        ball_mesh.shape, ball_mesh.texture, 
        ball.position + Vec3f(0,0,0),
        Vec3f(1,1,1) * ball.radius
    );
}

// Player ne devrait pas être là, mais je sais pas comment résoudre ça
void drawLevel(Level& level, Player& player, double time) {
    static Mesh wall_mesh = makeWallMesh();
    static Geometry bonus_mesh = createSphere(4);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    int size = level.length/2;
    for (int i = 0; i < size; i++) {
        float wall_distance = 2*(i+.5);
        float distance_to_player = abs(player.racket.position.z - wall_distance);
        float wall_light = 1 - distance_to_player / 10;
        for(int j = 0; j < 16; j++) {
            set_coord(wall_mesh.shape.colors, j, wall_light, wall_light, wall_light, 1);
        }
        draw3DObject(
            wall_mesh.shape, wall_mesh.texture,
            Vec3f(0, 0, wall_distance),
            Vec3f(level.width, -level.height, 1)
        );
    }

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    for(auto& box : level.obstacles) {
        drawAABB(box);
    }
    for(auto& bonus : level.bonus) {
        PhysicsAABB hitbox = bonus.getHitbox();
        drawAABB(hitbox);
        if(bonus.specs->is_victory || bonus.is_picked) {
            continue;
        }
        draw3DObject(bonus_mesh, bonus.position, bonus.specs->size / 2, Vec3f(0, 1, 1), time * 2 * 180 / M_PI);
    }
}

void drawRacket(Racket& racket) {
    static Geometry racket_mesh = makeRacketMesh();

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    drawAABB(racket.hitbox);
}

