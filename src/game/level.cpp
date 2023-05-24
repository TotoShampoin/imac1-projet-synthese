#include "game/level.h"
#include "IHM/objects.h"

#include <fstream>

#define file_read(file, data) file.read(reinterpret_cast<char*>(&data), sizeof(data))
#define file_write(file, data) file.write(reinterpret_cast<char*>(&data), sizeof(data))


Level::Level(float l) {
    length = l;
    walls.push_back(Obstacle {
        Vec3f(-LEVEL_WIDTH, -LEVEL_HEIGHT, -1),
        Vec3f( LEVEL_WIDTH,  LEVEL_HEIGHT, 1)
    });
    walls.push_back(Obstacle {
        Vec3f(-LEVEL_WIDTH, -LEVEL_HEIGHT, length),
        Vec3f( LEVEL_WIDTH,  LEVEL_HEIGHT, length+1)
    });
    walls.push_back(Obstacle {
        Vec3f(-LEVEL_WIDTH, -LEVEL_HEIGHT-.5, 0),
        Vec3f( LEVEL_WIDTH, -LEVEL_HEIGHT   , length)
    });
    walls.push_back(Obstacle {
        Vec3f(-LEVEL_WIDTH,  LEVEL_HEIGHT   , 0),
        Vec3f( LEVEL_WIDTH,  LEVEL_HEIGHT+.5, length)
    });
    walls.push_back(Obstacle {
        Vec3f(-LEVEL_WIDTH-.5, -LEVEL_HEIGHT, 0),
        Vec3f(-LEVEL_WIDTH   ,  LEVEL_HEIGHT, length)
    });
    walls.push_back(Obstacle {
        Vec3f( LEVEL_WIDTH   ,  LEVEL_HEIGHT, 0),
        Vec3f( LEVEL_WIDTH+.5, -LEVEL_HEIGHT, length)
    });

    bonus.push_back(Bonus(VICTORY, Vec3f(0, 0, length)));
}

Level::Level(const char* file_path) {
	std::fstream file (file_path, std::ios::in | std::fstream::binary);
	if(!file) {
		throw std::runtime_error(std::string(file_path) + " not found");
	}

    unsigned long obstacles_nb, bonus_nb;
    
	file_read(file, length);
	file_read(file, obstacles_nb);
	file_read(file, bonus_nb);

    for(size_t i = 0; i < obstacles_nb; i++) {
        Obstacle tmp_obs (Vec3f(0,0,0), Vec3f(0,0,0));
        file_read(file, tmp_obs.boundA.x);
        file_read(file, tmp_obs.boundA.y);
        file_read(file, tmp_obs.boundA.z);
        file_read(file, tmp_obs.boundB.x);
        file_read(file, tmp_obs.boundB.y);
        file_read(file, tmp_obs.boundB.z);
        obstacles.push_back(tmp_obs);
    }

    bonus.push_back(Bonus(VICTORY, Vec3f(0, 0, length)));
    for(size_t i = 0; i < bonus_nb; i++) {
        TypeID tmp_type;
        Vec3f tmp_pos;
        file_read(file, tmp_type);
        file_read(file, tmp_pos.x);
        file_read(file, tmp_pos.y);
        file_read(file, tmp_pos.z);
        Bonus tmp_bonus (tmp_type, tmp_pos);
        bonus.push_back(tmp_bonus);
    }
}

Mesh makeWallMesh() {
    return {
        .shape = createWall(),
        .texture = Image("assets/textures/walls.png"),
    };
}
