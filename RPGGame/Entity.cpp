#include "Entity.h"

Entity::Entity() {
    //no tiene constructor porque es una clase abstracta
    //se inicializan en las clases hijas
}

Entity::~Entity() {
}

void Entity::initTexture(std::string path)
{
    if(!this->texture.loadFromFile(path))
    {
        std::cout << "ERROR: COULD NOT LOAD TEXTURE: " << path << std::endl;
    }
}

void Entity::initSprite()
{
    this->sprite.setTexture(this->texture);
}

void Entity::initHitbox(float width, float height)
{
    this->hitbox.setSize(sf::Vector2f(width, height));
    this->hitbox.setFillColor(sf::Color::Transparent);
    this->hitbox.setOutlineColor(sf::Color::Red);
    this->hitbox.setOutlineThickness(1.f);
}

void Entity::setPosition(const float x, const float y) {
    this->sprite.setPosition(x, y);
    // Puede ser sobreescrito
    this->hitbox.setPosition(x, y); 
}

const sf::Vector2f Entity::getPosition() const {
    return this->sprite.getPosition();
}

const sf::FloatRect Entity::getGlobalBounds() const {
    return this->sprite.getGlobalBounds();
}

sf::Sprite& Entity::getSprite() {
    return this->sprite;
}

const sf::FloatRect Entity::getHitboxBounds() const {
    return this->hitbox.getGlobalBounds();
}
