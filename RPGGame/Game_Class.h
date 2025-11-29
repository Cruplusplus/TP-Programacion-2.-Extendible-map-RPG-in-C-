#pragma once
#include "Game_MapTiles.h"
#include "Jugador.h"
#include "Habitacion.h"
#include "DungeonGenerator.h"
#include "SaveManager.h"
#include "HUD.h"
#include "MainMenu.h"

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
    DungeonGenerator* dungeonGen;
    HUD* hud;
    MainMenu* mainMenu;
    sf::Vector2i currentRoomCoords;
    unsigned int seed;
    std::map<std::pair<int, int>, Habitacion*> roomsMap;

    enum GameState {
        STATE_MENU,
        STATE_PLAYING,
        STATE_GAMEOVER,
        STATE_VICTORY
    };
    GameState gameState;

    std::map<std::string, sf::Keyboard::Key> keyboardMappings;
    std::map<std::string, sf::Mouse::Button> mouseMappings;

    sf::Font font;
    sf::Text gameOverText;

//Inicializadores
    void initVariables();
    void initWindow();
    void initInput();
    void initTileSheet();
    void initPersonajes();
    void initHabitacion();
    void initFonts();
//
public:
    //Constrc - Destrc
    Juego();
    virtual ~Juego();

    void resetGame();

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
    
    void saveGame(int slot);
    void loadGame(int slot);
    //}
};
//}

