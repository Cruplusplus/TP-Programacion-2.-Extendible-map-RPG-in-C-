#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Entity {
protected:
    sf::Sprite sprite;
    sf::RectangleShape hitbox;

public:
    Entity();
    virtual ~Entity();

    virtual void update() = 0;
    virtual void render(sf::RenderTarget& target) = 0;

    virtual void setPosition(const float x, const float y);
    virtual const sf::Vector2f getPosition() const;
    virtual const sf::FloatRect getGlobalBounds() const;
    virtual sf::Sprite& getSprite();
    virtual const sf::FloatRect getHitboxBounds() const;
};
