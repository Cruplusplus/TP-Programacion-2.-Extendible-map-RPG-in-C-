#pragma once
#include "GameConfig.h"

class Tile
{
private:
    sf::Sprite sprite;
    bool colision;

public:
    Tile(const int& grid_pos_x, const int& grid_pos_y, const int& tile_size,
         sf::Texture* textureSheet, sf::IntRect textureRect);

    const sf::FloatRect getGlobalBounds() const;
    const bool getColision() const;

    void update();
    void render(sf::RenderTarget& target);
};

enum tipoTiles {PISO = 1, PARED_LEFT, PARED_TOP, PARED_CORNER, ROCA};

class TileMap
{
private:
    //Como una matriz, un vector que contiene vectores asi podemos hacer 2 dimensiones
    std::vector< std::vector<Tile*> > tiles;
    sf::Texture* tileSheet;
    unsigned tileSize;

public:
    TileMap();
    TileMap(unsigned width, unsigned height, sf::Texture* tile_sheet, unsigned tile_size);
    ~TileMap();

    //inline funcs
    const unsigned& getTileSize() const;

    void addTile(unsigned x, unsigned y, int tileType);
    void removeTile(unsigned x, unsigned y);

    bool checkCollision(sf::FloatRect rect);
    void update();
    void render(sf::RenderTarget& target);
};
