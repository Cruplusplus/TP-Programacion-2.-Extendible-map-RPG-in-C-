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
    this->currentFrame = sf::IntRect(20, 0, 18, 40);
    this->sprite.setTextureRect(this->currentFrame);
    this->sprite.setScale(sf::Vector2f(2.5f, 2.5f));
    this->sprite.setOrigin(10, 10);
}

void Personajes::initAnimations()
{
    this->animationTimer.restart();
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
    EstadoPersonaje::Normal;
    this->initVariables();
    this->initTexture();
    this->initSprite();
    this->initAnimations();
    this->velocidad = 2.f;
}

Jugador::getHp()
{
    return this->hp;
}


void Jugador::updateMovement()
{
    this->animState = PLAYER_ANIMATION_STATES::IDLE;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        this->sprite.setScale(-2.5f, 2.5f);
        mover(-1.f, 0.f);
        this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        this->sprite.setScale(2.5f, 2.5f);
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

void Jugador::updateAnimations()
{
    if(this->animState == PLAYER_ANIMATION_STATES::IDLE)
    {
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.5f)
        {
            this->currentFrame.top = 0.f;
            this->currentFrame.left = 20.f;

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        return;
    }

    if(this->animState == PLAYER_ANIMATION_STATES::MOVING_RIGHT)
    {
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.5f)
        {
            this->currentFrame.top = 0.f;
            this->currentFrame.left += 20.f;
            if(this->currentFrame.left >= 80.f)
                this->currentFrame.left = 0;

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        return;
    }

    if(this->animState == PLAYER_ANIMATION_STATES::MOVING_LEFT)
    {
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.5f)
        {
            this->currentFrame.top = 0.f;
            this->currentFrame.left += 20.f;
            if(this->currentFrame.left >= 80.f)
                this->currentFrame.left = 0;

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        return;
    }

    if(this->animState == PLAYER_ANIMATION_STATES::MOVING_UP)
    {
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.5f)
        {
            this->currentFrame.top = 43.f;
            this->currentFrame.left += 20.f;
            if(this->currentFrame.left >= 184.f)
                this->currentFrame.left = 124.f;

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
        return;
    }

}

void Jugador::update()
{
    this->updateMovement();
    this->updateAnimations();
}

void Jugador::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);
}

//}
