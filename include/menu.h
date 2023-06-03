#include <vector>
#include <string>

#include "IHM/window.h"

struct MenuData {
    std::string level_path;
};

MenuData menu(Window& win);
