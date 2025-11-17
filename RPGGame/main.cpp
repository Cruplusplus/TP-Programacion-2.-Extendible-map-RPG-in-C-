#include <iostream>
#include "Game_Class.h"
#include "json.hpp"
using namespace std;
//using namespace sf;

int main() {
    srand(static_cast<unsigned> (time(0)));

    //Inicializar el engine
    Juego juego;

    //Game loop

    while (juego.gameRunning() && !juego.getFinalizarJuego())
    {
        //update
        juego.update();
        //render
        juego.render();
        //drawgame
    }

return 0;
}
