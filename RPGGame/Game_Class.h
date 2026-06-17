#pragma once
#include "Game_MapTiles.h"
#include "Jugador.h"
#include "Habitacion.h"
#include "DungeonGenerator.h"
#include "SaveManager.h"
#include "HUD.h"
#include "MainMenu.h"

#include <algorithm>
#include <set>

class Juego{
private:

    VisualConfig visualConfig;
    sf::RenderWindow *window;
    sf::VideoMode videoMode;
    sf::Event ev;

    sf::Texture tileSheet;

    bool finalizarJuego;
    int vida;

    Jugador* jugador;
    Habitacion* habitacionActual;
    DungeonGenerator* dungeonGen;
    HUD* hud;
    MainMenu* mainMenu;
    sf::Vector2i currentRoomCoords;
    unsigned int seed;
    std::map<std::pair<int, int>, Habitacion*> roomsMap;
    std::set<std::pair<int, int>> clearedRooms;

    enum GameState {
        STATE_MENU,
        STATE_PLAYING,
        STATE_PAUSE_MENU,
        STATE_GAMEOVER,
        STATE_VICTORY
    };
    GameState gameState;

    std::map<std::string, sf::Keyboard::Key> keyboardMappings;
    std::map<std::string, sf::Mouse::Button> mouseMappings;

    sf::Font font;
    sf::Text gameOverText;

    //muejejejeeje
    std::vector<sf::Keyboard::Key> konamiSequence;
    int konamiIndex;

    void initVariables();
    void initWindow();
    void initInput();
    void initTileSheet();
    void initPersonajes();
    void initHabitacion();
    void initFonts();
public:
    Juego();
    virtual ~Juego();

    void resetGame();
    void nextDungeonFloor();

    const bool gameRunning() const;
    const bool getFinalizarJuego() const;
    const sf::RenderWindow& getWindow() const;

    void updateInput();
    void handleMenuInput(sf::Keyboard::Key key);

    void pollEvents();

    void updatePersonajes();
    void updateCollision();

    void update();

    void render();

    int getPuntos();
    
    void saveGame(int slot);
    void loadGame(int slot);
};

