#include "game/player.h"
#include "game/level.h"
#include "IHM/draw.h"
#include "common.h"

static const float CAMERA_SHIFT = .1;

void placeCamera(Player& player, Level& level) {
    gluLookAt(
        player.racket.position.x * CAMERA_SHIFT, player.racket.position.y * CAMERA_SHIFT, player.racket.position.z - 2,
        0, 0, level.length,
        0, 1, 0
    );
}

void setColor(Geometry& geo, float r, float g, float b, float a) {
    for(int j = 0; j < geo.vert_nb; j++) {
        set_coord(geo.colors, j, r, g, b, a);
    }
}
void setAlpha(Geometry& geo, float a) {
    for(int j = 0; j < geo.vert_nb; j++) {
        geo.colors[j*4 + 3] = a;
    }
}

static const float LIGHT_STRENGTH = 1.5;
static const float STRETCH = 1 / 5.;
void vertexLight(Geometry& geo, Vec3f offset, Vec3f scale, std::vector<Vec3f> positions, float r, float g, float b) {
    GLfloat* colors = geo.colors;
    for(int i = 0; i < geo.vert_nb; i++) {
        int ic = i * 3;
        Vec3f vert = Vec3f(
            geo.vertices[ic+0] * scale.x,
            geo.vertices[ic+1] * scale.y,
            geo.vertices[ic+2] * scale.z
        ) + offset;
        float light = 0;
        for(const auto& position : positions) {
            float dist2 = (vert - position).norm2();
            light += LIGHT_STRENGTH / (STRETCH * dist2 - STRETCH + 1);
        }
        light /= positions.size();
        set_coord(geo.colors, i, r * light, g * light, b * light, 1);
    }
}
void modelLight(Geometry& geo, Vec3f offset, std::vector<Vec3f> positions, float r, float g, float b) {
    Vec3f vert = offset;
    float light = 0;
    for(const auto& position : positions) {
        float dist2 = (vert - position).norm2();
        light += LIGHT_STRENGTH / (STRETCH * dist2 - STRETCH + 1);
    }
    light /= positions.size();
    
    GLfloat* colors = geo.colors;
    for(int i = 0; i < geo.vert_nb; i++) {
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

#define NB_TEXTURES 5
void drawLevel(Level& level, Player& player, double time) {
    static Mesh wall_mesh = makeWallMesh();
    static Geometry obstacle_mesh = createPlane();
    static Image obstacle_textures[NB_TEXTURES] = {
        Image("assets/textures/obstacles/elies_fight.jpg"),
        Image("assets/textures/obstacles/elies_oh.jpg"),
        Image("assets/textures/obstacles/leaugane.jpg"),
        Image("assets/textures/obstacles/maureen.jpg"),
        Image("assets/textures/obstacles/shampoing.jpg")
    };

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    int size = level.length/2;

    std::vector<Vec3f> light_sources;
    light_sources.push_back(player.racket.position);
    light_sources.push_back(player.ball.position);

    for (int i = 0; i < size; i++) {
        float wall_distance = 2*(i+.5);
        vertexLight(wall_mesh.shape, Vec3f(0, 0, wall_distance), Vec3f(1, -1, 1), light_sources, 1, 1, 1);
        draw3DObject(
            wall_mesh.shape, wall_mesh.texture,
            Vec3f(0, 0, wall_distance),
            Vec3f(level.width, -level.height, 1)
        );
    }

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    for(auto& bonus : level.bonus) {
        if(bonus.specs->is_victory || bonus.is_picked) {
            continue;
        }
        float distance = (bonus.position - player.racket.position).z;
        modelLight(*bonus.specs->mesh, bonus.position, light_sources, 1, .5, .25);
        setAlpha(*bonus.specs->mesh, remap(distance, -1, 1, 0, 1));
        draw3DObject(*bonus.specs->mesh, bonus.position, bonus.specs->size / 2, Vec3f(0, 1, 0), time * 2 * 180 / M_PI);
    }
    int index = 0;
    for(auto& box : level.obstacles) {
        Vec3f middle = box.middle();
        Vec3f size = box.size();
        Vec3f offset1 = Vec3f(0, 0, -size.z);
        Vec3f offset2 = Vec3f(0, 0,  size.z);

        float distance = (middle - player.racket.position).z;

        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        vertexLight(obstacle_mesh, middle, size, light_sources, 1, 1, 1);
        setAlpha(obstacle_mesh, remap(distance, -1, 1, 0, 1));
        draw3DObject(obstacle_mesh, obstacle_textures[index], middle - offset2, size);
        draw3DObject(obstacle_mesh, obstacle_textures[index], middle - offset1, size);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        index = (index + 1) % NB_TEXTURES;
    }
}

void drawRacket(Racket& racket) {
    static Geometry racket_mesh = makeRacketMesh();

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    drawAABB(racket.hitbox);
}

