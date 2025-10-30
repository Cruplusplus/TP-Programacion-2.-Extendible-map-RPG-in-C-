#pragma once
#include "Game_Struct_Clases.h"

class Tile
{
private:
    sf::Sprite sprite;
    const bool damaging;

public:
    Tile(sf::Texture* textureSheet, sf::IntRect textureRect, bool damaging = false);

    const sf::FloatRect getGlobalBounds() const;

    void update();
    void render(sf::RenderTarget& target);
};


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

    void addTile(unsigned x, unsigned y);
    void removeTile(unsigned x, unsigned y);

    void update();
    void render(sf::RenderTarget& target);
};
