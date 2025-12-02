#include "Proyectil.h"
#include <iostream>

Proyectil::Proyectil(float x, float y, sf::Vector2f dir, float speed, int dmg, bool isEnemy)
{
    this->direction = dir;
    this->speed = speed;
    this->damage = dmg;
    this->enemyProjectile = isEnemy;

    this->initTexture("Sprites ejemplo/player_sprites.png");
    this->initSprite();
    
    //un cuadradito chiquito de momento esta bien
    this->sprite.setTextureRect(sf::IntRect(0, 0, 10, 10)); 
    this->sprite.setColor(isEnemy ? sf::Color::Red : sf::Color::Yellow);
    
    this->initHitbox(10.f, 10.f);
    this->hitbox.setPosition(x, y);
    this->sprite.setPosition(x, y);
    
    this->setPosition(x, y);
}

Proyectil::~Proyectil()
{
}

void Proyectil::update()
{
    this->sprite.move(this->direction * this->speed);
    this->hitbox.move(this->direction * this->speed);
}

void Proyectil::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);
    // target.draw(this->hitbox); // Debug
}

bool Proyectil::isEnemy() const
{
    return this->enemyProjectile;
}

int Proyectil::getDamage() const
{
    return this->damage;
}
