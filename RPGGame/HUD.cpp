#include "HUD.h"
#include <iostream>

HUD::HUD() {

    if(!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }

    this->hpText.setFont(font);
    this->hpText.setCharacterSize(20);
    this->hpText.setPosition(10.f, 10.f);
    this->hpText.setFillColor(sf::Color::Red);

    this->coinText.setFont(font);
    this->coinText.setCharacterSize(20);
    this->coinText.setPosition(10.f, 40.f);
    this->coinText.setFillColor(sf::Color::Yellow);

    this->floorsText.setFont(font);
    this->floorsText.setCharacterSize(20);
    this->floorsText.setPosition(10.f, 70.f);
    this->floorsText.setFillColor(sf::Color::White);
    
    this->itemText.setFont(font);
    this->itemText.setCharacterSize(20);
    this->itemText.setPosition(10.f, 100.f);
    this->itemText.setFillColor(sf::Color::Cyan);
}

void HUD::update(Jugador* jugador) {
    this->hpText.setString("HP: " + std::to_string(jugador->getHp()) + "/" + std::to_string(jugador->getMaxHp()));
    this->coinText.setString("Coins: " + std::to_string(jugador->getCoins()));
    this->floorsText.setString("Floor: " + std::to_string(jugador->getLevelPiso()));
    std::string items = "Items: ";
    if(jugador->hasItem(ITEM_DASH)) items += "Dash ";
    if(jugador->hasItem(ITEM_KAMIKAZE)) items += "Kamikaze ";
    if(jugador->hasItem(ITEM_BOW)) items += "Bow ";
    if(jugador->hasItem(ITEM_ARCOMIKAZE)) items += "Arcomikaze ";
    this->itemText.setString(items);
}

void HUD::render(sf::RenderTarget& target) {
    target.draw(this->hpText);
    target.draw(this->coinText);
    target.draw(this->floorsText);
    target.draw(this->itemText);
}
