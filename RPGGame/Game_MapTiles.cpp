#include "Game_MapTiles.h"

//=============MAPA================

Tile::Tile(sf::Texture* textureSheet, sf::IntRect textureRect, bool damaging)
    : damaging(damaging)
{
    damaging = false;
    this->sprite.setTexture(*textureSheet);
    this->sprite.setTextureRect(textureRect);
}

const sf::FloatRect Tile::getGlobalBounds() const
{
    return this->sprite.getGlobalBounds();
}

void Tile::update()
{

}

void Tile::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);
}

//===TileMap===

TileMap::TileMap()
{
    this->tileSheet = NULL;
    this->tileSize = 0;
}

TileMap::TileMap(unsigned width, unsigned height, sf::Texture* tile_sheet, unsigned tile_size)
{
    this->tileSheet = tile_sheet;
    this->tileSize = tile_size;

    this->tiles.resize(width);
    for(int i = 0; i < this->tiles.size(); i++)
    {
        this->tiles[i].resize(height, nullptr);
    }
}

TileMap::~TileMap()
{
    for(int i = 0; i < this->tiles.size(); i++)
    {
        for(int j = 0; j < this->tiles[i].size(); j++)
        {
            delete this->tiles[i][j];
            this->tiles[i][j] = nullptr;
        }
    }
}

void TileMap::addTile(unsigned x, unsigned y)
{
    if(x < this->tiles.size())
    {
        if(y < this->tiles[x].size())
        {
            if(this->tiles[x][y] == nullptr){
                this->tiles[x][y] = new Tile(this->tileSheet, sf::IntRect(0,0,
                                            this->tileSize, this->tileSize), false);
                                            //ya esta el false pero quiero remarcarlo
            }
        }
    }
}

void TileMap::removeTile(unsigned x, unsigned y)
{
    if(x < this->tiles.size())
    {
        if(y < this->tiles[x].size())
        {
            if(this->tiles[x][y] != nullptr)
            {
                delete this->tiles[x][y];
                this->tiles[x][y] = nullptr;
            }
        }
    }
}

void TileMap::update()
{
    for(int i = 0; i < this->tiles.size(); i++)
    {
        for(int j = 0; j < this->tiles[i].size(); j++)
        {
            if(this->tiles[i][j] != nullptr)
                this->tiles[i][j]->update();
        }
    }
}

void TileMap::render(sf::RenderTarget& target)
{
    for(int i = 0; i < this->tiles.size(); i++)
    {
        for(int j = 0; j < this->tiles[i].size(); j++)
        {
            if(this->tiles[i][j] != nullptr)
                this->tiles[i][j]->render(target);
        }
    }
}
