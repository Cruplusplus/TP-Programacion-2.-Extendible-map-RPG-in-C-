#include "Habitacion.h"
#include "Enemigos.h"
#include "Jugador.h"

Habitacion::Habitacion(sf::Texture* tile_sheet)
{
    this->tileSheet = tile_sheet;
    this->tileMap = nullptr;

    if (!this->backgroundTexture.loadFromFile("Sprites ejemplo/suelo_habitacion.png"))
    {
        std::cout << "ERROR: No se pudo cargar el fondo de la habitacion" << std::endl;
    }
    this->backgroundSprite.setTexture(this->backgroundTexture);
    this->backgroundSprite.setScale(3.f, 3.f);
    // this->backgroundSprite.setPosition(x, y);

    this->initTileMap();
    this->initEnemigos();
}

Habitacion::~Habitacion()
{
    delete this->tileMap;

    // Borramos todos los enemigos del vector
    for (auto* enemigo : this->enemigos)
    {
        delete enemigo;
    }
}

void Habitacion::initTileMap()
{
    const unsigned TILE_SIZE = 50;
    const int MAP_WIDTH = 20;//valores de ejemplo
    const int MAP_HEIGHT = 10;
    int levelData[MAP_HEIGHT][MAP_WIDTH] =
    {
        {4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4},
        {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {3,0,0,0,0,0,5,5,5,5,5,0,0,0,0,0,0,0,3},
        {3,0,0,0,0,0,5,5,5,5,5,0,0,0,0,0,0,0,3},
        {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
        {4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4}
    };

    this->tileMap = new TileMap(MAP_WIDTH, MAP_HEIGHT, this->tileSheet, TILE_SIZE);

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            int tileType = levelData[y][x];
            if (tileType != 0)
            {
                this->tileMap->addTile(x, y, tileType);
            }
        }
    }
}

void Habitacion::initEnemigos()
{
    // this->enemigos.push_back(new Duende(300.f, 300.f, &this->texturaDuende));
    this->enemigos.push_back(new Duende(200.f, 200.f));
}

void Habitacion::update(Jugador* jugador)
{
    this->tileMap->update();

    for (auto* enemigo : this->enemigos)
    {
        Duende* duende = dynamic_cast<Duende*>(enemigo);
        if (duende)
        {
            duende->updateIA(jugador);
        }

        //animaciones y etc

        enemigo->update();
    }
}

void Habitacion::renderFondo(sf::RenderTarget& target)
{
    target.draw(this->backgroundSprite);

    this->tileMap->render(target);
}
