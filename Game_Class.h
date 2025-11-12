#pragma once
#include "Game_Struct_Clases.h"
#include <memory> /// ayuda a guardar en memoria de forma dinamica

//{Motor grafico del juego
class Juego{
private:
    //{----VARIABLES----
//ventana
    VisualConfig visualConfig;
    std::unique_ptr<sf::RenderWindow> window; /// solicita memoria y la borra automaticamente sin necesidad de delete
    sf::VideoMode videoMode;
    sf::Event ev;

//Logica del juego
    bool finalizarJuego;

//objetos del juego
    //Jugador
    std::unique_ptr<Jugador> jugador; /// lo mismo al borrar la memoria, por lo tanto no ncecesitaria un destructor

//Inicializadores
    void initVariables();
    void initWindow();
    void initPersonajes();
//}
public:
    //Constrc - Destrc
    Juego();

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

