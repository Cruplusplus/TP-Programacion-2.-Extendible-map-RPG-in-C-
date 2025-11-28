#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include <string>

enum ItemType {
    ITEM_NONE = 0,
    ITEM_KAMIKAZE,
    ITEM_DASH,
    ITEM_BOW,
    ITEM_ARCOMIKAZE
};

enum PickupType {
    PICKUP_HEART = 0,
    PICKUP_SPECTRAL_HEART,
    PICKUP_COIN,
    PICKUP_KEY
};

class Item : public Entity {
public:
    ItemType type;
    std::string name;
    
    Item(ItemType t, std::string n) : type(t), name(n) {}
    virtual ~Item() {}
    
    void update() override {}
    void render(sf::RenderTarget& target) override { target.draw(this->sprite); }
};

class Pickup : public Entity {
public:
    PickupType type;
    bool collected;

    Pickup(PickupType t, float x, float y);
    virtual ~Pickup() {}

    void update() override {}
    void render(sf::RenderTarget& target) override;
    const sf::FloatRect getGlobalBounds() const override;
};
