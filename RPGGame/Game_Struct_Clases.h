#pragma once

#include "Visual_Clases.h"

enum PLAYER_ANIMATION_STATES {IDLE = 0, MOVING_LEFT, MOVING_RIGHT, MOVING_UP, MOVING_DOWN};

class Personajes
{
protected:
    int id;
    int hp;
    int dmg;
    int lvl;
    std::string nombre;

    sf::Texture textureSheet;
    sf::Sprite sprite;
    float velocidad;
    sf::Clock animationTimer;

    //Animacion
    short animState;
    sf::IntRect currentFrame;

    //
    enum class TipoPersonaje {Jugador, Enemigos, Boss};
    TipoPersonaje tipo;

    enum class EstadoPersonaje {Normal, Atacando, Moviendose};
    EstadoPersonaje estado;
    //

    void initVariables();
    void initTexture();
    void initSprite();
    void initAnimations();

public:
    Personajes(int _id, int _hp, int _dmg, int _lvl,
               std::string _nombre);
    virtual ~Personajes();

    virtual void mover(const float dx, const float dy);
    /*
    virtual void atacar(Personajes& objetivo);
    virtual void recibirDanio(int cantidad);
    virtual void dibujar(sf::RenderWindow& window);*/
};

class Inventario
{
private:
    int idJugador;
    int idObjeto;
    int cantidad;
public:

};

class Jugador : public Personajes
{
private:
    /*
    vector<Inventario> inventario;
    */
public:
    Jugador();


    int getHp();

    void updateMovement();
    void updateAnimations();
    void update();
    //Visual
    void render(sf::RenderTarget& target);
};

class Enemigos : public Personajes
{
private:
    int idEnemigo;
    int hpBase;
    int dmgBase;
    int lvlMin;
    int lvlMax;
public:
    Enemigos();

};

class Mazmorras
{
private:
    int idChunk;
    int chanceAparicion;
    int dificultad;

    /*
    vector<Mazmorras> chunks;
    vector<Enemigos> enemigos;
    vector<Objetos> tesoros;
    */
public:

};

class Mapa
{
private:
    int idMapa;
    int seed;
    int pisoActual;
    int totalPisos;
    int chunksTotales;
    int enemigosTotales;
public:

};

class Objetos
{
private:
    int idObjeto;
    std::string nombre;
    int tipo;
public:

};
