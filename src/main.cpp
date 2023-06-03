#include "IHM/audio.h"
#include "main.h"
#include "common.h"
#include "menu.h"
#include "game.h"

int main(int argc, const char* argv[]) {
    Window win {800, 600, "fenetre"};
    AudioContext channel;
    
    // TODO: L'écran titre et l'écran de menu

    while(!win.shouldClose()) {
        MenuData data = main_menu(win);
        main_game(data.level_path.c_str(), win);
    }

    return 0;
}
