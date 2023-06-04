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

void setColor(Geometry& geo, float r, float g, float b, float a) {
    for(int j = 0; j < geo.vert_nb; j++) {
        set_coord(geo.colors, j, r, g, b, a);
    }
}

// Broken ?
void vertexLight(Geometry& geo, Vec3f position, float dist_max, float r, float g, float b) {
    GLfloat* colors = geo.colors;
    for(int i = 0; i < geo.vert_nb; i++) {
        int ic = i * 3;
        Vec3f vert {
            geo.vertices[ic+0],
            geo.vertices[ic+1],
            geo.vertices[ic+2]
        };
        float light = 1 - (vert * position) / dist_max;
        set_coord(geo.colors, i, r * light, g * light, b * light, 1);
    }
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
        setColor(wall_mesh.shape, wall_light, wall_light, wall_light, 1);
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
        draw3DObject(*bonus.specs->mesh, bonus.position, bonus.specs->size / 2, Vec3f(0, 1, 0), time * 2 * 180 / M_PI);
    }
}

void drawRacket(Racket& racket) {
    static Geometry racket_mesh = makeRacketMesh();

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    drawAABB(racket.hitbox);
}

