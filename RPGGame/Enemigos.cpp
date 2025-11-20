#include "Enemigos.h"
#include "Jugador.h"
#include <cmath>
#include <iostream>

Enemigos::Enemigos(int _id, int _hp, int _dmg, int _lvl, std::string _nombre)
    : Personajes(_id, _hp, _dmg, _lvl, _nombre)
{
    this->tipo = TipoPersonaje::Enemigos;
    this->velocidad = 1.f;

    this->ladoPreferido = 1.f;
    this->enModoEvasion = false;
    this->lastPosition = this->getPosition();
}

Enemigos::~Enemigos()
{
}

void Enemigos::updateIA(Jugador* jugador)
{
    float distMovida = std::abs(this->getPosition().x - this->lastPosition.x) +
                       std::abs(this->getPosition().y - this->lastPosition.y);

    bool isStuck = (std::abs(this->velocidadVector.x) + std::abs(this->velocidadVector.y) > 0.1f)
                    && (distMovida < 0.5f);

    sf::Vector2f delta = jugador->getPosition() - this->sprite.getPosition();
    float distJugador = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    sf::Vector2f dirJugador = (distJugador != 0) ? (delta / distJugador) : sf::Vector2f(0,0);

    // --- ATAQUE ---
    if (distJugador < this->getHitboxBounds().width + 5.f)
    {
        this->animState = PLAYER_ANIMATION_STATES::ATTACK;
        this->velocidadVector = sf::Vector2f(0,0);
        if (this->animationTimer.getElapsedTime().asSeconds() >= 1.f) {
            jugador->recibirDanio(1);
            std::cout << "PUM!" << std::endl;
            this->animationTimer.restart();
        }
        this->lastPosition = this->getPosition();
        return;
    }

    // -- ESTADOS --

    if (!this->enModoEvasion)
    {
        // --- ESTADO NORMAL ---
        if (isStuck)
        {
            this->enModoEvasion = true;
            this->timerEvasion.restart();

            if (this->ladoPreferido > 0)
                this->dirEvasion = sf::Vector2f(-dirJugador.y, dirJugador.x);
            else
                this->dirEvasion = sf::Vector2f(dirJugador.y, -dirJugador.x);

            this->velocidadVector = this->dirEvasion;
        }
        else
        {
            // Camino libre: Perseguir
            this->velocidadVector = dirJugador;
        }
    }
    else
    {
        // --- ESTADO EVASIÓN ---
        this->velocidadVector = this->dirEvasion;

        if (isStuck && this->timerEvasion.getElapsedTime().asSeconds() > 0.2f)
        {
            this->ladoPreferido *= -1.f;
            if (this->ladoPreferido > 0)
                this->dirEvasion = sf::Vector2f(-dirJugador.y, dirJugador.x);
            else
                this->dirEvasion = sf::Vector2f(dirJugador.y, -dirJugador.x);

            this->velocidadVector = this->dirEvasion;
            this->timerEvasion.restart();
        }

        if (this->timerEvasion.getElapsedTime().asSeconds() > 0.8f)
        {
            this->enModoEvasion = false;
        }
    }

    // ANIMACIONES
    if (std::abs(this->velocidadVector.x) > std::abs(this->velocidadVector.y))
    {
        if (this->velocidadVector.x > 0) this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
        else this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
    }
    else
    {
        if (this->velocidadVector.y > 0) this->animState = PLAYER_ANIMATION_STATES::MOVING_DOWN;
        else this->animState = PLAYER_ANIMATION_STATES::MOVING_UP;
    }

    this->lastPosition = this->getPosition();
}

void Enemigos::update()
{
    if (std::abs(this->velocidadVector.x) > std::abs(this->velocidadVector.y))
    {
        if (this->velocidadVector.x > 0) this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
        else this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
    }
    else
    {
        if (this->velocidadVector.y > 0) this->animState = PLAYER_ANIMATION_STATES::MOVING_DOWN;
        else this->animState = PLAYER_ANIMATION_STATES::MOVING_UP;
    }

    // logica de sprites/frames (updateAnimations)
}

void Enemigos::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);
}

Duende::Duende(float x, float y)
    : Enemigos(2, 3, 1, 1, "Duende")
{
    this->initTexture("Sprites ejemplo/player_sprites.png");
    this->initSprite();

    this->initHitbox(19.f, 20.f);
    this->hitbox.setOrigin(this->sprite.getOrigin());
    this->hitbox.setScale(this->sprite.getScale());

    this->setPosition(x, y);
}

Duende::~Duende()
{
}

void Duende::update()
{
    Enemigos::update();
}
