#include <iostream>
#include "Game.h"
using namespace std;
//using namespace sf;

int main()
{

    //Inicializar el engine
    Juego juego;

    //Esto crea un loop game

    while (juego.gameRunning())
    {
        //update
        juego.update();
        //render
        juego.render();

        //drawgame
    }
    return 0;
}
