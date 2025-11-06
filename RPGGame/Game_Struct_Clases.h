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

    //fisicas
    sf::Vector2f velocidadVector;

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

    //acc
    void setPosition(const float x, const float y);

    const sf::Vector2f getPosition() const;
    const sf::FloatRect getGlobalBounds() const;
    sf::Vector2f& getVelocidadVector();
    //

    virtual void mover(const float dx, const float dy);

    virtual void update() = 0;
    virtual void render(sf::RenderTarget& target) = 0; //Esto quiere decir que obliga a sus hijos a tener las fx
};

class Objetos
{
private:
    int idObjeto;
    enum tipoDeObjeto {Pickup = 0, PowerUp};
public:

};
