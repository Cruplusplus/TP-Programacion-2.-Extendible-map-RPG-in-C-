#include "HUD.h"
#include <iostream>

HUD::HUD() {
    // Cargar fuente (usando fuente por defecto o una si está disponible, por ahora solo intento cargar una o morir)
    // En un escenario real, se empaquetaría una fuente. Se renderizará texto si falla
    // SFML necesita un archivo de fuente.
    // Uso una del sistema

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

    this->keyText.setFont(font);
    this->keyText.setCharacterSize(20);
    this->keyText.setPosition(10.f, 70.f);
    this->keyText.setFillColor(sf::Color::White);
    
    this->itemText.setFont(font);
    this->itemText.setCharacterSize(20);
    this->itemText.setPosition(10.f, 100.f);
    this->itemText.setFillColor(sf::Color::Cyan);
}

void HUD::update(Jugador* jugador) {
    this->hpText.setString("HP: " + std::to_string(jugador->getHp()) + "/" + std::to_string(jugador->getMaxHp()));
    this->coinText.setString("Coins: " + std::to_string(jugador->getCoins()));
    this->keyText.setString("Keys: " + std::to_string(jugador->getKeys()));
    
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
    target.draw(this->keyText);
    target.draw(this->itemText);
}
