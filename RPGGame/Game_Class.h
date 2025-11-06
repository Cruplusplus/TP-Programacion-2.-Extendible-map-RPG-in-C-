#pragma once
#include "Game_MapTiles.h"
#include "Jugador.h"
#include "Habitacion.h"

#include <algorithm>

//{Motor grafico del juego
class Juego{
private:
//ventana
    VisualConfig visualConfig;
    sf::RenderWindow *window;
    sf::VideoMode videoMode;
    sf::Event ev;

    sf::Texture tileSheet;

//Logica del juego
    bool finalizarJuego;
    int vida;

//objetos del juego
    Jugador* jugador;
    Habitacion* habitacionActual;

    std::map<std::string, sf::Keyboard::Key> keyboardMappings;
    std::map<std::string, sf::Mouse::Button> mouseMappings;

//Inicializadores
    void initVariables();
    void initWindow();
    void initInput();
    void initTileSheet();
    void initPersonajes();
    void initHabitacion();
//
public:
    //Constrc - Destrc
    Juego();
    virtual ~Juego();

    //Accesors
    const bool gameRunning() const;
    const bool getFinalizarJuego() const;
    const sf::RenderWindow& getWindow() const;

    //{Funcs
    //void spawnEnemigo();
    void updateInput();

    void pollEvents();

    //Updates
    void updatePersonajes();
    void updateCollision();

    void update();

    void render();

    //Getters
    int getPuntos();
    //}
};
//}

