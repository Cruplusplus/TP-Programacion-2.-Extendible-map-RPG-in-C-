#include "Items.h"

Pickup::Pickup(PickupType t, float x, float y) {
    this->type = t;
    this->setPosition(x, y);
    this->collected = false;
}

void Pickup::render(sf::RenderTarget& target) {
    if(collected) return;
    
    sf::RectangleShape shape(sf::Vector2f(15.f, 15.f));
    shape.setPosition(this->getPosition());
    
    switch(type) {
        case PICKUP_HEART: shape.setFillColor(sf::Color::Red); break;
        case PICKUP_SPECTRAL_HEART: shape.setFillColor(sf::Color::Blue); break;
        case PICKUP_COIN: shape.setFillColor(sf::Color::Yellow); break;
        case PICKUP_STAT_UP: shape.setFillColor(sf::Color(192, 192, 192)); break;
        case PICKUP_ITEM_BOW: 
            shape.setFillColor(sf::Color::Green); 
            shape.setSize(sf::Vector2f(30.f, 30.f)); 
            break;
        case PICKUP_ITEM_KAMIKAZE: 
            shape.setFillColor(sf::Color(255, 128, 0)); 
            shape.setSize(sf::Vector2f(30.f, 30.f)); 
            break; 
        case PICKUP_ITEM_DASH: 
            shape.setFillColor(sf::Color::Cyan); 
            shape.setSize(sf::Vector2f(30.f, 30.f)); 
            break;
    }
    
    target.draw(shape);
}

const sf::FloatRect Pickup::getGlobalBounds() const {
    return sf::FloatRect(this->getPosition().x, this->getPosition().y, 15.f, 15.f);
}
