#include "Game_MapTiles.h"

//=============MAPA================

Tile::Tile(const int& grid_pos_x, const int& grid_pos_y, const int& tile_size,
           sf::Texture* textureSheet, sf::IntRect textureRect)
{
    this->colision = false;
    this->sprite.setTexture(*textureSheet);
    this->sprite.setTextureRect(textureRect);
    this->sprite.setPosition(float(grid_pos_x * tile_size), float(grid_pos_y * tile_size));
}

const sf::FloatRect Tile::getGlobalBounds() const
{
    return this->sprite.getGlobalBounds();
}

const bool Tile::getColision() const
{
    return this->colision;
}

void Tile::update()
{}

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

void TileMap::addTile(unsigned x, unsigned y, int tileType)
{
    if(x >= this->tiles.size() && x >= 0)
    {
        return;
    }
    if(y >= this->tiles[x].size() && y >= 0)
    {
        return;
    }
    if(this->tiles[x][y] != nullptr)
    {
        return;
    }

    sf::IntRect textureRect;
    switch (tileType)
    {
    case 0:
        // 0 seria bg
        break;
    case tipoTiles::PISO:
        textureRect = sf::IntRect(63, 52, this->tileSize, this->tileSize);
        break;
    case tipoTiles::PARED_LEFT:
        textureRect = sf::IntRect(89, 89, this->tileSize, this->tileSize);
        break;
    case tipoTiles::PARED_TOP:
        textureRect = sf::IntRect(62, 0, this->tileSize, this->tileSize);
        break;
    case tipoTiles::PARED_CORNER:
        textureRect = sf::IntRect(0, 0, this->tileSize, this->tileSize);
        break;
    case tipoTiles::ROCA:
        textureRect = sf::IntRect(256, 0, this->tileSize, this->tileSize);
        break;
    default:
        textureRect = sf::IntRect(0, 0, this->tileSize, this->tileSize);
        break;
    }

    this->tiles[x][y] = new Tile(x, y, tileSize, this->tileSheet,
                                 textureRect);
}

void TileMap::removeTile(unsigned x, unsigned y)
{
    if(x >= this->tiles.size())
    {
        return;
    }
    if(y >= this->tiles[x].size())
    {
        return;
    }
    if(this->tiles[x][y] == nullptr)
    {
        return;
    }

    delete this->tiles[x][y];
    this->tiles[x][y] = nullptr;
}


bool TileMap::checkCollision(sf::FloatRect rect)
{
    if (this->tiles.empty()) return false;

    int left = rect.left / this->tileSize;
    int right = (rect.left + rect.width) / this->tileSize;
    int top = rect.top / this->tileSize;
    int bottom = (rect.top + rect.height) / this->tileSize;

    //x si acaso seteo los parametros que deberia cumplir
    left = std::max(0, left);
    right = std::min((int)this->tiles.size() - 1, right);
    top = std::max(0, top);
    bottom = std::min((int)this->tiles[0].size() - 1, bottom);

    for (int x = left; x <= right; ++x)
    {
        for (int y = top; y <= bottom; ++y)
        {
            if (this->tiles[x][y] != nullptr)
            {
                //chequear tile
                if (rect.intersects(this->tiles[x][y]->getGlobalBounds()))
                {
                    return true;
                }
            }
        }
    }
    return false;
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

const unsigned& TileMap::getTileSize() const { return this->tileSize; }
