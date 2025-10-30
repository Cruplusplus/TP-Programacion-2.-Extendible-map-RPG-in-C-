#pragma once
#include "Game_MapTiles.h"

//{Motor grafico del juego
class Juego{
private:
    //{----VARIABLES----
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
    TileMap* tileMap;

//Inicializadores
    void initVariables();
    void initWindow();
    void initTileSheet();
    void initPersonajes();
    void initTileMap();
//}
public:
    //Constrc - Destrc
    Juego();
    virtual ~Juego();

    //Accesors
    const bool gameRunning() const;
    const bool getFinalizarJuego() const;

    //{Funcs
    //void spawnEnemigo();

    void pollEvents();

    //Updates
    void updatePersonajes();
    void updateCollision();
    void updateTileMap();

    void update();

    //Renders
    void renderPersonajes(sf::RenderTarget& target);
    void renderTileMap();

    void render();

    //Getters
    int getPuntos();
    const sf::RenderWindow& getWindow() const;
    //}
};
//}

