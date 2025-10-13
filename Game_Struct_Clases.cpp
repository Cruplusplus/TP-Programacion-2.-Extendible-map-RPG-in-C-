#include "Game_Struct_Clases.h"

Personajes::Personajes(int _id, int _hp, int _dmg, int _lvl, std::string _nombre, std::string archivoSprites)
{
    this->id = _id;
    this->hp = _hp;
    this->dmg = _dmg;
    this->lvl = _lvl;
    this->nombre = _nombre;

    if(!(this->textura.loadFromFile(archivoSprites)))
    {
        std::cout << "ERROR CON LA CARGA DE TEXTURA";
        return;
    }
    this->sprite.setTexture(this->textura);
}

/*
const int idJugador = 1;
const int vidaInicialJugador = 10;
const int dmgInicialJugador = 1;
const int lvlInicial = 1;
const std::string nombreJugador = "Knight";
*/

Jugador::Jugador()
    : Personajes(1 , 10, 1, 1, "Knight", "Sprites ejemplo/player_sprites_ejemplo.png") {}

Jugador::getHp() { return this->hp; }



void Jugador::update()
{
    this->sprite.setTextureRect(sf::IntRect(32*48, 0, 32, 48));
    float velocidad = 5.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        this->sprite.move(0, -velocidad);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        this->sprite.move(0, velocidad);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        this->sprite.move(-velocidad, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        this->sprite.move(velocidad, 0);
}

void Jugador::draw(sf::RenderTarget& target)
{
    target.draw(this->sprite);
}

