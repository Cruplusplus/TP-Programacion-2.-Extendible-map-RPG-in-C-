#include "Enemigos.h"
#include <cmath>

Enemigos::Enemigos(int _id, int _hp, int _dmg, int _lvl, std::string _nombre)
    : Personajes(_id, _hp, _dmg, _lvl, _nombre)
{
    this->tipo = TipoPersonaje::Enemigos;
}

Enemigos::~Enemigos()
{
}

void Enemigos::update()
{

}

void Enemigos::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);
}

Duende::Duende(float x, float y)
        : Enemigos(2, 3, 1, 1, "Duende")
{
    this->tipo = TipoPersonaje::Enemigos;

    this->initTexture("Sprites ejemplo/player_sprites.png");
    this->initSprite();

    this->initHitbox(19.f, 20.f);
    this->hitbox.setOrigin(this->sprite.getOrigin());
    this->hitbox.setScale(this->sprite.getScale());

    this->velocidad = 1.f;
    //this->initAnimations();
    this->setPosition(x, y);
}

Duende::~Duende()
{
}

void Duende::updateIA(Jugador* jugador)
{
    sf::Vector2f direction = jugador->getPosition() - this->sprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance != 0) {
        direction = direction / distance;
    }

    float SIGHT_RANGE = 100.f;
    float ATTACK_RANGE = 20.f;

    if (distance < ATTACK_RANGE)
    {
        this->animState = PLAYER_ANIMATION_STATES::ATTACK;

        //logica de ataque
        if (this->animationTimer.getElapsedTime().asSeconds() >= 1.f)
        {
            std::cout << "ataca enemigo" << std::endl;
            jugador->recibirDanio(1);
            this->animationTimer.restart();
        }

    }
    else if (distance < SIGHT_RANGE)
    {
        this->velocidadVector.x = direction.x;
        this->velocidadVector.y = direction.y;

        //Animacion
        if (direction.x > 0)
            this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
        else
            this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
    }
    else
    {
        this->animState = PLAYER_ANIMATION_STATES::IDLE;
        this->velocidadVector.x = 0.f;
        this->velocidadVector.y = 0.f;
    }
}

void Duende::update()
{

}
