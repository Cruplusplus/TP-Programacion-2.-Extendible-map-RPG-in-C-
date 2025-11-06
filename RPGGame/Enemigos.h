#include "Game_Struct_Clases.h"
#pragma once

class Enemigos : public Personajes
{
private:
public:
    Enemigos(int _id, int _hp, int _dmg, int _lvl, std::string _nombre);
    virtual ~Enemigos();

    virtual void update() override;
    virtual void render(sf::RenderTarget& target) override;
};

class Duende : public Enemigos
{
    private:
    public:
    Duende(float x, float y);
    virtual ~Duende();

    //Funciones
    void updateIA(sf::Vector2f playerPos);
    void update() override;
};
