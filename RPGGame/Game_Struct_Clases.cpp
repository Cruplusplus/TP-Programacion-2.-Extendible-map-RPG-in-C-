#include "Game_Struct_Clases.h"

void Personajes::initVariables()
{
    this->animState = PLAYER_ANIMATION_STATES::IDLE;
}

void Personajes::initTexture()
{
    if(!this->textureSheet.loadFromFile("Sprites ejemplo/player_sprites.png"))
    {
        std::cout << "ERROR CON LA CARGA DE TEXTURA: Personajes::initTexture";
    }
    this->textureSheet.setSmooth(false);
}

void Personajes::initSprite()
{
    this->sprite.setTexture(this->textureSheet);
    this->currentFrame = sf::IntRect(20, 45, 19, 44);
    this->sprite.setTextureRect(this->currentFrame);
    this->sprite.setScale(sf::Vector2f(2.5f, 2.5f));
}

void Personajes::initAnimations()
{
    this->animationTimer.restart();
    this->animationSwitch = true;
}

Personajes::Personajes(int _id, int _hp, int _dmg, int _lvl,
                       std::string _nombre)
{
    this->id = _id;
    this->hp = _hp;
    this->dmg = _dmg;
    this->lvl = _lvl;
    this->nombre = _nombre;

}

Personajes::~Personajes()
{

}

void Personajes::mover(const float dx, const float dy)
{
    this->sprite.move(this->velocidad * dx, this->velocidad * dy);
}

const sf::Vector2f Personajes::getPosition() const { return this->sprite.getPosition(); }

const sf::FloatRect Personajes::getGlobalBounds() const { return this->sprite.getGlobalBounds(); }

sf::Vector2f& Personajes::getVelocidadVector() { return this->velocidadVector; }

void Personajes::setPosition(const float x, const float y) { this->sprite.setPosition(x, y); }
