#include "Game_Struct_Clases.h"
#pragma once

class Jugador : public Personajes
{
private:
public:
    Jugador(const float x, const float y);

    //setters
    void resetAnimTimer();

    const bool getAnimSwitch();

    int getHp() const;

    void updateMovement();
    void atacar(Personajes *enemigo);
    void updateAnimations();
    void update() override;
    //Visual
    void render(sf::RenderTarget& target) override;
};
