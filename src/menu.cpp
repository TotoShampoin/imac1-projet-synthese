#include "menu.h"

#include "common.h"
#include "IHM/window.h"
#include "IHM/objects.h"
#include "IHM/draw.h"
#include "IHM/audio.h"
#include "game/level.h"

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#define LEVEL_PATH "assets/levels/"
#define MENU_OFFSET 4

#define file_read(file, data) file.read(reinterpret_cast<char*>(&data), sizeof(data))

struct LevelFile {
    std::string file, name;
};

std::string getLevelName(std::string file_path) {
	std::fstream file (file_path, std::ios::in | std::fstream::binary);
	if(!file) {
		throw std::runtime_error(std::string(file_path) + " not found");
	}
    char name[LEVEL_NAME_LENGTH];
    for(size_t i = 0; i < LEVEL_NAME_LENGTH; i++) {
        file_read(file, name[i]);
    }
    return name;
}

std::vector<LevelFile> searchLevels() {
    std::vector<LevelFile> files;
    for(const auto& entry : std::filesystem::directory_iterator(LEVEL_PATH)) {
        std::string entry_file = entry.path();
        std::string entry_name = getLevelName(entry_file);
        const char* entry_c = entry_file.c_str();
        if(endsWith(entry_c, ".lvl")) {
            files.push_back({entry_file, entry_name});
        }
    }
    return files;
}

MenuData main_menu(Window& win) {
    static Font font ("assets/textures/fonts/minecraft.png", 16, 16, 8, 16, '\0', 16, 16);
    font.texture.setFilter(GL_NEAREST, GL_NEAREST);

    std::vector<LevelFile> levels = searchLevels();

    int cursor = 0;
    bool selected = false;

    win.on_mouse_move = [&cursor](double xpos, double ypos) {
        int x = xpos * -300 + 300;
        int y = ypos * -300 + 300;
        cursor = (y) / 32 - MENU_OFFSET;
    };
    win.on_mouse_button = [&cursor, &selected, &levels](int button, int action, int mods) {
        if(action == GLFW_RELEASE && (cursor >= 0 && cursor < levels.size())) {
            selected = true;
        }
    };
    
    while(!(win.shouldClose() || selected)) {
        win.pollEvents();
        win.clear();

        use2dMode(win);

        glPushMatrix();
        glScaled(1/300., 1/300., 1/300.);
        glTranslated(-300, -300, 0);
            glColor3f(1, 1, 1);
            draw2DText("IMAC Corridor", font, Coord2D {
                Vec2f(0, (MENU_OFFSET - 2) * 32),
                Vec2f(16, 16),
                0
            }, false);
            for(int i = 0; i < 12 && i < levels.size(); i++) {
                const char* level = levels[i].name.c_str();
                if(cursor == i) {
                    glColor3f(0, 0, 1);
                    draw2DBox(
                        Vec2f(300, (MENU_OFFSET + cursor) * 32 + 16),
                        Vec2f(300, 16)
                    );
                    glColor3f(0, 1, 1);
                } else {
                    glColor3f(1, 1, 1);
                }
                draw2DText(level, font, Coord2D {
                    Vec2f(0, (MENU_OFFSET + i) * 32),
                    Vec2f(8, 8),
                    0
                }, false);
            }

        glPopMatrix();

        win.refresh();
        timer();
    }

    return MenuData {
        .level_path = levels[cursor].file
    };
}
