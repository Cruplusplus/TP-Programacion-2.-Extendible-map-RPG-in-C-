#pragma once
#include <SFML/Graphics.hpp>
#include "Jugador.h"

class HUD {
private:
    sf::Font font;
    sf::Text hpText;
    sf::Text coinText;
    sf::Text keyText;
    sf::Text itemText;

public:
    HUD();
    void update(Jugador* jugador);
    void render(sf::RenderTarget& target);
};
