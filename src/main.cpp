#include "main.h"
#include "common.h"
#include "game.h"

int main(int argc, const char* argv[]) {
    Window win {800, 600, "fenetre"};

    // TODO: L'écran titre et l'écran de menu

    startGame("assets/levels/test.lvl", win);

    return 0;
}
