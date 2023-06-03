#include "main.h"
#include "common.h"
#include "menu.h"
#include "game.h"

int main(int argc, const char* argv[]) {
    Window win {800, 600, "fenetre"};
    
    // TODO: L'écran titre et l'écran de menu

    MenuData data = menu(win);
    startGame(data.level_path.c_str(), win);

    return 0;
}
