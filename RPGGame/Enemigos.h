<<<<<<< Updated upstream
#include "Game_Struct_Clases.h"
=======
#include "Character.h"
#include "Jugador.h"
>>>>>>> Stashed changes
#pragma once
#include "Jugador.h"

class Enemigos : public Character
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
    void updateIA(Jugador* jugador);
    void update() override;
};

class Orco : public Enemigos
{
public:
    Orco(float x, float y);
    virtual ~Orco();
    void update() override;
};

class Hada : public Enemigos
{
public:
    Hada(float x, float y);
    virtual ~Hada();
    void update() override;
    void curarAliados(std::vector<Enemigos*>& enemigos);
};

class Estatua : public Enemigos
{
public:
    Estatua(float x, float y);
    virtual ~Estatua();
    void update() override;
    // Special flag to check if it should die
    bool checkDeathCondition(int numEnemies);
};

class Hechicero : public Enemigos
{
private:
    sf::Clock attackTimer;
public:
    Hechicero(float x, float y);
    virtual ~Hechicero();
    void update() override;
    void attack(Jugador* jugador);
};
