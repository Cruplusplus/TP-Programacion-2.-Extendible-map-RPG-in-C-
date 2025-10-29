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

//================JUGADOR================{

Jugador::Jugador()
    : Personajes(1, 10, 1, 1, "Knight")
{
    TipoPersonaje::Jugador;
    this->initVariables();
    this->initTexture();
    this->initSprite();
    this->initAnimations();
    this->velocidad = 2.f;
}

const bool Jugador::getAnimSwitch()
{
    bool animSwitch = this->animationSwitch;

    if(this->animationSwitch)
        {this->animationSwitch = false;}
    return animSwitch;
}

const sf::FloatRect Jugador::getGlobalBounds() const
{
    return this->sprite.getGlobalBounds();
}

const sf::Vector2f Jugador::getPosition() const
{
    return this->sprite.getPosition();
}

void Jugador::setPosition(const float x, const float y)
{
    this->sprite.setPosition(x, y);
}

Jugador::getHp()
{
    return this->hp;
}

void Jugador::resetAnimTimer()
{
    this->animationTimer.restart();
    this->animationSwitch = true;
}

//================MOVIMIENTO==================

void Jugador::updateMovement()
{
    this->animState = PLAYER_ANIMATION_STATES::IDLE;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::K))
    {
        this->animState = PLAYER_ANIMATION_STATES::ATTACK;
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        mover(-1.f, 0.f);
        this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        mover(1.f, 0.f);
        this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        mover(0.f, -1.f);
        this->animState = PLAYER_ANIMATION_STATES::MOVING_UP;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        mover(0.f, 1.f);
        this->animState = PLAYER_ANIMATION_STATES::MOVING_DOWN;
    }

}

//================ANIMACIONES==================

void Jugador::updateAnimations()
{
    if(this->animState == PLAYER_ANIMATION_STATES::IDLE)
    {
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.2f)
        {
            this->currentFrame.top = 45.f;
            this->currentFrame.left = 20.f;

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        return;
    }


    if(this->animState == PLAYER_ANIMATION_STATES::MOVING_LEFT)
    {
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.2f)
        {
            this->currentFrame.top = 0.f;
            this->currentFrame.left += 20.f;
            if(this->currentFrame.left >= 80.f)
                this->currentFrame.left = 0;

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }

        this->sprite.setScale(-2.5f, 2.5f);
        this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2.5f, 0.f);

        return;
    }

    if(this->animState == PLAYER_ANIMATION_STATES::MOVING_RIGHT)
    {
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.2f || getAnimSwitch())
        {
            this->currentFrame.top = 0.f;
            this->currentFrame.left += 20.f;
            if(this->currentFrame.left >= 80.f)
                this->currentFrame.left = 0;

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }

        this->sprite.setScale(2.5f, 2.5f);
        this->sprite.setOrigin(0.f, 0.f);

        return;
    }

    if(this->animState == PLAYER_ANIMATION_STATES::MOVING_UP)
    {

        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.2f)
        {
            this->currentFrame.top = 89.f;
            this->currentFrame.left += 20.f;
            if(this->currentFrame.left >= 78.f)
                this->currentFrame.left = 0.f;

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        return;
    }

    if(this->animState == PLAYER_ANIMATION_STATES::MOVING_DOWN)
    {
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.2f)
        {
            this->currentFrame.top = 45.f;
            this->currentFrame.left += 20.f;
            if(this->currentFrame.left >= 78.f)
                this->currentFrame.left = 0.f;

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        return;
    }

    if(this->animState == PLAYER_ANIMATION_STATES::ATTACK)
    {
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.2)
        {
            /*
            this->currentFrame = sf::IntRect(0, 148, 27, 37);

            this->currentFrame = sf::IntRect(29, 148, 18, 49);

            this->currentFrame = sf::IntRect(49, 148, 31, 44);*/

            this->currentFrame = sf::IntRect(81, 148, 49, 36);

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        this->currentFrame = sf::IntRect(20, 0, 19, 44);
    }
}

//=============UPDATE Y RENDER=============

void Jugador::update()
{
    this->updateMovement();
    this->updateAnimations();
}

void Jugador::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);

    //testeando

    sf::CircleShape circ;
    circ.setFillColor(sf::Color::Red);
    circ.setRadius(2.f);
    circ.setPosition(this->sprite.getPosition());

    target.draw(circ);
}

//}

//=============MAPA================

Tile::Tile(sf::Texture& textureSheet, sf::IntRect textureRect)
{
    this->sprite.setTexture(textureSheet);
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
