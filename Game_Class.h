#pragma once
#include "Game_Struct_Clases.h"

#include <vector>

//{Motor grafico del juego
class Juego{
private:
    //{----VARIABLES----
//ventana
    sf::RenderWindow *window;
    sf::VideoMode videoMode;
    sf::Event ev;

//Logica del juego
    bool finalizarJuego;
    int vida;

//objetos del juego
    //Jugador
    Jugador jugador;

    std::vector<sf::Sprite> enemigos;
    sf::Sprite enemigo;

//Inicializadores
    void initVariables();
    void initWindow();
    void initPersonajes();
//}
public:
    //Constrc - Destrc
    Juego();
    virtual ~Juego();

    //Accesors
    const bool gameRunning() const;
    const bool getFinalizarJuego() const;

    //{Funcs
    void spawnEnemigo();

    void pollEvents();

    void updatePersonajes();
    void update();

    void renderPersonajes(sf::RenderTarget& target);
    void render();

    int getPuntos();
    //}
};
//}

