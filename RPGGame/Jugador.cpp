#include "Jugador.h"

//================JUGADOR================{

Jugador::Jugador(const float x, const float y)
    : Personajes(1, 10, 1, 1, "Knight")
{
    TipoPersonaje::Jugador;
    this->initVariables();
    this->initTexture();
    this->initSprite();
    this->initAnimations();
    this->velocidad = 2.f;

    this->setPosition(x, y);

}

const bool Jugador::getAnimSwitch()
{
    bool animSwitch = this->animationSwitch;

    if(this->animationSwitch)
        {this->animationSwitch = false;}
    return animSwitch;
}



int Jugador::getHp() { return this->hp; }

void Jugador::resetAnimTimer()
{
    this->animationTimer.restart();
    this->animationSwitch = true;
}

//================MOVIMIENTO==================

void Jugador::updateMovement()
{
    this->velocidadVector.x = 0.f;
    this->velocidadVector.y = 0.f;

    this->animState = PLAYER_ANIMATION_STATES::IDLE;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::K))
    {
        this->animState = PLAYER_ANIMATION_STATES::ATTACK;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        this->velocidadVector.x = 1.f;
        this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        this->velocidadVector.x = -1.f;
        this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        this->velocidadVector.y = -1.f;
        this->animState = PLAYER_ANIMATION_STATES::MOVING_UP;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        this->velocidadVector.y = 1.f;
        this->animState = PLAYER_ANIMATION_STATES::MOVING_DOWN;
    }

}

//================ANIMACIONES==================

void Jugador::updateAnimations()
{
    if (this->animState == PLAYER_ANIMATION_STATES::ATTACK)
    {
        if (this->animationTimer.getElapsedTime().asSeconds() >= 0.1f || getAnimSwitch())
        {

            this->currentFrame.top = 135.f;
            this->currentFrame.left = 0.f;

            this->sprite.setScale(2.5f, 2.5f);
            this->sprite.setOrigin(0.f, 0.f);

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }

        return;
    }

    if(this->animState == PLAYER_ANIMATION_STATES::IDLE)
    {
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.2f || getAnimSwitch())
        {
            this->currentFrame.top = 45.f;
            this->currentFrame.left = 20.f;

            this->sprite.setScale(2.5f, 2.5f);
            this->sprite.setOrigin(0.f, 0.f);

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }

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

            this->sprite.setScale(2.5f, 2.5f);
            this->sprite.setOrigin(0.f, 0.f);

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }

        return;
    }

    if(this->animState == PLAYER_ANIMATION_STATES::MOVING_LEFT)
    {
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.2f || getAnimSwitch())
        {
            this->currentFrame.top = 0.f;
            this->currentFrame.left += 20.f;
            if(this->currentFrame.left >= 80.f)
                this->currentFrame.left = 0;

            this->sprite.setScale(-2.5f, 2.5f);
            this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2.5f, 0.f);

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }

        return;
    }

    if(this->animState == PLAYER_ANIMATION_STATES::MOVING_UP)
    {

        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.2f || getAnimSwitch())
        {
            this->currentFrame.top = 89.f;
            this->currentFrame.left += 20.f;
            if(this->currentFrame.left >= 78.f)
                this->currentFrame.left = 0.f;

            this->sprite.setScale(2.5f, 2.5f);
            this->sprite.setOrigin(0.f, 0.f);

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }

        return;
    }

    if(this->animState == PLAYER_ANIMATION_STATES::MOVING_DOWN)
    {
        if(this->animationTimer.getElapsedTime().asSeconds() >= 0.2f || getAnimSwitch())
        {
            this->currentFrame.top = 45.f;
            this->currentFrame.left += 20.f;
            if(this->currentFrame.left >= 78.f)
                this->currentFrame.left = 0.f;

            this->sprite.setScale(2.5f, 2.5f);
            this->sprite.setOrigin(0.f, 0.f);

            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }

        return;
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
}

//}
