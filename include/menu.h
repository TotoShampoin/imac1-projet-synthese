#include <vector>
#include <string>

#include "IHM/window.h"

struct MenuData {
    std::string level_path;
};

MenuData main_menu(Window& win);
