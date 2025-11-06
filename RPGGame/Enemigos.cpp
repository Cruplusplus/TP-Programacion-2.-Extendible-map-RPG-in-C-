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
    //IA
}

void Enemigos::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);
}

Duende::Duende(float x, float y)
        : Enemigos(2, 3, 1, 1, "Duende")
{
    this->tipo = TipoPersonaje::Enemigos;
    /*
    if (!this->textureSheet.loadFromFile("Sprites ejemplo/duende_sprites.png"))
    {
        std::cout << "ERROR CON LA CARGA DE TEXTURA: Duende" << std::endl;
    }

    this->textureSheet.setSmooth(false);

    this->sprite.setTexture(this->textureSheet);
    this->currentFrame = sf::IntRect(0, 0, 16, 16);*/
    this->sprite.setTextureRect(this->currentFrame);
    this->sprite.setScale(sf::Vector2f(2.5f, 2.5f));
    this->sprite.setPosition(x, y);

    this->velocidad = 1.f;
    //this->initAnimations();
}

Duende::~Duende()
{
}

void Duende::updateIA(sf::Vector2f playerPos)
{
    sf::Vector2f direction = playerPos - this->sprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance != 0) {
        direction = direction / distance;
    }

    float SIGHT_RANGE = 300.f;
    float ATTACK_RANGE = 50.f;

    if (distance < ATTACK_RANGE)
    {
        this->animState = PLAYER_ANIMATION_STATES::ATTACK;

        //logica de ataque que no se hacer

    }
    else if (distance < SIGHT_RANGE)
    {
        this->mover(direction.x, direction.y);

        //Animacion
        if (direction.x > 0)
            this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
        else
            this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
    }
    else
    {
        this->animState = PLAYER_ANIMATION_STATES::IDLE;
    }
}

void Duende::update()
{

}
