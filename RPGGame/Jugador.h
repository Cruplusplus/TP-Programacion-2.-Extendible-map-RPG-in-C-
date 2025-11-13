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

    virtual void recibirDanio(int danio);
    int getDmg() const;
    int getHp() const;

    void updateMovement();
    void updateAnimations();
    void update() override;
    //Visual
    void render(sf::RenderTarget& target) override;
};
