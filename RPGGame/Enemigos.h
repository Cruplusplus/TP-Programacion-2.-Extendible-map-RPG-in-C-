#include "Game_Struct_Clases.h"
#include "Jugador.h"
#pragma once


class Jugador;

class Enemigos : public Personajes
{
protected:
    sf::Vector2f lastPosition;

    sf::Clock timerEvasion;
    bool enModoEvasion;
    sf::Vector2f dirEvasion;
    float ladoPreferido;

public:
    Enemigos(int _id, int _hp, int _dmg, int _lvl, std::string _nombre);
    virtual ~Enemigos();

    void updateIA(Jugador* jugador);

    virtual void update() override;

    virtual void render(sf::RenderTarget& target) override;
};

class Duende : public Enemigos
{
public:
    Duende(float x, float y);
    virtual ~Duende();

    void update() override;
};
