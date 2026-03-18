#include <iostream>
#include "Game_Class.h"
#include "json.hpp"
using namespace std;
//using namespace sf;

int main() {
    srand(static_cast<unsigned> (time(0)));

    Juego juego;

    while (juego.gameRunning() && !juego.getFinalizarJuego())
    {
        juego.update();
        juego.render();
    }

return 0;
}

