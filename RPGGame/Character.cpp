#include "Character.h"

void Character::initVariables()
{
    this->animState = PLAYER_ANIMATION_STATES::IDLE;
}

void Character::initTexture(const std::string ubicacionSprite)
{
    if(!this->textureSheet.loadFromFile(ubicacionSprite))
    {
        std::cout << "ERROR CON LA CARGA DE TEXTURA: Character::initTexture" << std::endl;
    }
    this->textureSheet.setSmooth(false);
}

void Character::initSprite()
{
    this->sprite.setTexture(this->textureSheet);
    this->currentFrame = sf::IntRect(20, 45, 19, 44);
    this->sprite.setTextureRect(this->currentFrame);
    this->sprite.setScale(sf::Vector2f(2.5f, 2.5f));
}

void Character::initHitbox(float width, float height)
{
    this->hitbox.setSize(sf::Vector2f(width, height));

    this->hitbox.setFillColor(sf::Color::Transparent);
    this->hitbox.setOutlineColor(sf::Color::Red);
    this->hitbox.setOutlineThickness(1.f);
}

void Character::initAnimations()
{
    this->animationTimer.restart();
    this->animationSwitch = true;
}

Character::Character(int _id, int _hp, int _dmg, int _lvl,
                       std::string _nombre)
{
    this->id = _id;
    this->hp = _hp;
    this->dmg = _dmg;
    this->lvl = _lvl;
    this->nombre = _nombre;

    this->initVariables();
}

Character::~Character()
{

}

void Character::mover(const float dx, const float dy)
{
    this->sprite.move(this->velocidad * dx, this->velocidad * dy);
    this->hitbox.move(this->velocidad * dx, this->velocidad * dy);
}

int Character::getHp() const {return this->hp;}

void Character::recibirDanio(int danio) { this->hp -= danio; }

sf::Vector2f& Character::getVelocidadVector() { return this->velocidadVector; }

void Character::setPosition(const float x, const float y)
{
    this->sprite.setPosition(x, y);
    this->hitbox.setPosition(x, y+60);
}

void Character::renderHitbox(sf::RenderTarget& target) { target.draw(this->hitbox); }
