#pragma once
#include "Entity.h"

class Proyectil : public Entity
{
private:
    sf::Vector2f direction;
    float speed;
    int damage;
    bool enemyProjectile; // True si es de enemigo, False si es del jugador
    
public:
    Proyectil(float x, float y, sf::Vector2f dir, float speed, int dmg, bool isEnemy);
    virtual ~Proyectil();

    void update() override;
    void render(sf::RenderTarget& target) override;

    bool isEnemy() const;
    int getDamage() const;
};
