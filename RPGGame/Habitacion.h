#pragma once

#include "Game_MapTiles.h"
#include "Enemigos.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Habitacion
{
private:
    sf::Texture* tileSheet;
    TileMap* tileMap;
    std::vector<Enemigos*> enemigos;

    //BG
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    void initTileMap();
    void initEnemigos();

public:
    Habitacion(sf::Texture* tile_sheet);
    virtual ~Habitacion();

    void update(Jugador* jugador); // Pasar la pos del jugador para la IA

    void renderFondo(sf::RenderTarget& target);

    TileMap* getTileMap() const { return this->tileMap; }

    std::vector<Enemigos*> getEnemigos() { return this->enemigos; }
};
