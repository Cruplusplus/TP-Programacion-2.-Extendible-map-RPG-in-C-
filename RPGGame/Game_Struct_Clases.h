#pragma once

#include "Visual_Clases.h"

enum PLAYER_ANIMATION_STATES {IDLE = 0, MOVING_LEFT, MOVING_RIGHT, MOVING_UP, MOVING_DOWN, ATTACK};

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
    bool animationSwitch;

    //Animacion
    short animState;
    sf::IntRect currentFrame;

    //
    enum class TipoPersonaje {Jugador, Enemigos, Boss};
    TipoPersonaje tipo;
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

class Jugador : public Personajes
{
private:
public:
    Jugador();

    const bool&  getAnimSwitch();
    const sf::FloatRect getGlobalBounds() const;
    const sf::Vector2f getPosition() const;

    //setters
    void resetAnimTimer();
    void setPosition(const float x, const float y);

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

class Tile
{
private:
    sf::Sprite sprite;

public:
    Tile(sf::Texture& textureSheet, sf::IntRect textureRect);

    const sf::FloatRect getGlobalBounds() const;

    void update();
    void render(sf::RenderTarget& target);
};


class TileMap
{
private:

public:
    TileMap();
    ~TileMap();
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
