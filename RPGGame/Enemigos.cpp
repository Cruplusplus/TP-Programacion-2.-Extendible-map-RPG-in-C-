#include "Enemigos.h"
#include <cmath>

Enemigos::Enemigos(int _id, int _hp, int _dmg, int _lvl, std::string _nombre)
    : Personajes(_id, _hp, _dmg, _lvl, _nombre)
{
    this->tipo = TipoPersonaje::Enemigos;

    this->velocidad = 1.f;

    this->isStuckX = false;
    this->isStuckY = false;
    this->lastPosition = this->getPosition();
    this->stuckTimerX.restart();
    this->stuckTimerY.restart();

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

void Duende::updateIA(Jugador* jugador)
{
    float distMovidaX = std::abs(this->getPosition().x - this->lastPosition.x);
    float distMovidaY = std::abs(this->getPosition().y - this->lastPosition.y);

    //variables de direccion y distancia
    sf::Vector2f direction = jugador->getPosition() - this->sprite.getPosition();
    float distancia = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distancia != 0) { direction = direction / distancia; }

    //=X=
    if (std::abs(this->velocidadVector.x) > 0.1f && distMovidaX < 0.1f)
    {
        this->isStuckX = true;
        this->stuckTimerX.restart();
    }
    else
    {
        this->isStuckX = false;
    }
    //=Y=
    if (std::abs(this->velocidadVector.y) > 0.1f && distMovidaY < 0.1f)
    {
        this->isStuckY = true;
        this->stuckTimerY.restart();
    }
    else
    {
        this->isStuckY = false;
    }

    float stuckThreshold = 0.1f;

    if (this->isStuckX && this->stuckTimerX.getElapsedTime().asSeconds() > stuckThreshold)
    {
        this->velocidadVector.x = 0.f;
        this->velocidadVector.y = direction.y;
        return;
    }
    else if (this->isStuckY && this->stuckTimerY.getElapsedTime().asSeconds() > stuckThreshold)
    {
        this->velocidadVector.y = 0.f;
        this->velocidadVector.x = direction.x;
        return;
    }

    //LOGICA DE IA

    float SIGHT_RANGE = 300.f;
    float ATTACK_RANGE = this->getHitboxBounds().width;

    if (distancia < ATTACK_RANGE)
    {
        this->animState = PLAYER_ANIMATION_STATES::ATTACK;
        this->velocidadVector = sf::Vector2f(0.f, 0.f);

        //logica de ataque
        if (this->animationTimer.getElapsedTime().asSeconds() >= 1.f)
        {
            std::cout << "ataca enemigo" << std::endl;
            jugador->recibirDanio(1);
            this->animationTimer.restart();
        }
    }
    else if (distancia < SIGHT_RANGE)
    {
        this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
        this->velocidadVector = direction;
    }
    else
    {
        this->animState = PLAYER_ANIMATION_STATES::IDLE;
        this->velocidadVector = sf::Vector2f(0.f, 0.f);
    }

    this->lastPosition = this->getPosition();
}

void Duende::update()
{

}
