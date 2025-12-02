#pragma once
#include "Entity.h"
#include "Game_MapTiles.h"

enum PLAYER_ANIMATION_STATES {IDLE = 0, MOVING_LEFT, MOVING_RIGHT, MOVING_UP, MOVING_DOWN, ATTACK};

class Character : public Entity {
protected:
    int id;
    int hp;
    int dmg;
    int lvl;
    std::string nombre;

    float velocidad;
    sf::Clock animationTimer;
    bool animationSwitch;

    sf::Vector2f velocidadVector;

    // Animacion
    short animState;
    sf::IntRect currentFrame;

    enum class TipoPersonaje {Jugador, Enemigos, Boss};
    TipoPersonaje tipo;

    void initVariables();
    void initSprite(); // Override/Extend
    void initAnimations();

public:
    Character(int _id, int _hp, int _dmg, int _lvl, std::string _nombre);
    virtual ~Character();

    sf::Vector2f& getVelocidadVector();
    int getHp() const;

    virtual void mover(const float dx, const float dy);
    virtual void recibirDanio(int danio);

    virtual void update() = 0;
    virtual void render(sf::RenderTarget& target) = 0;

    void renderHitbox(sf::RenderTarget& target);
    
    void setPosition(const float x, const float y) override;
};
