#include "Enemigos.h"
#include <cmath>

//================ENEMIGOS================

Enemigos::Enemigos(int _id, int _hp, int _dmg, int _lvl, std::string _nombre)
    : Character(_id, _hp, _dmg, _lvl, _nombre)
{
    this->tipo = TipoPersonaje::Enemigos;
}

Enemigos::~Enemigos()
{
}

void Enemigos::updateIA(Jugador* jugador)
{
    float distMovida = std::abs(this->getPosition().x - this->lastPosition.x) +
                       std::abs(this->getPosition().y - this->lastPosition.y);

    bool isStuck = (std::abs(this->velocidadVector.x) + std::abs(this->velocidadVector.y) > 0.1f)
                    && (distMovida < 0.5f);

    sf::Vector2f delta = jugador->getPosition() - this->sprite.getPosition();
    float distJugador = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    sf::Vector2f dirJugador = (distJugador != 0) ? (delta / distJugador) : sf::Vector2f(0,0);

    // --- ATAQUE ---
    if (distJugador < this->getHitboxBounds().width + 5.f)
    {
        this->animState = PLAYER_ANIMATION_STATES::ATTACK;
        this->velocidadVector = sf::Vector2f(0,0);
        if (this->animationTimer.getElapsedTime().asSeconds() >= 1.f) {
            jugador->recibirDanio(1);
            std::cout << "PUM!" << std::endl;
            this->animationTimer.restart();
        }
        this->lastPosition = this->getPosition();
        return;
    }

    // -- ESTADOS --

    if (!this->enModoEvasion)
    {
        // --- ESTADO NORMAL ---
        if (isStuck)
        {
            this->enModoEvasion = true;
            this->timerEvasion.restart();

            if (this->ladoPreferido > 0)
                this->dirEvasion = sf::Vector2f(-dirJugador.y, dirJugador.x);
            else
                this->dirEvasion = sf::Vector2f(dirJugador.y, -dirJugador.x);

            this->velocidadVector = this->dirEvasion;
        }
        else
        {
            // Camino libre: Perseguir
            this->velocidadVector = dirJugador;
        }
    }
    else
    {
        // --- ESTADO EVASIÓN ---
        this->velocidadVector = this->dirEvasion;

        if (isStuck && this->timerEvasion.getElapsedTime().asSeconds() > 0.2f)
        {
            this->ladoPreferido *= -1.f;
            if (this->ladoPreferido > 0)
                this->dirEvasion = sf::Vector2f(-dirJugador.y, dirJugador.x);
            else
                this->dirEvasion = sf::Vector2f(dirJugador.y, -dirJugador.x);

            this->velocidadVector = this->dirEvasion;
            this->timerEvasion.restart();
        }

        if (this->timerEvasion.getElapsedTime().asSeconds() > 0.8f)
        {
            this->enModoEvasion = false;
        }
    }

    // ANIMACIONES
    if (std::abs(this->velocidadVector.x) > std::abs(this->velocidadVector.y))
    {
        if (this->velocidadVector.x > 0) this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
        else this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
    }
    else
    {
        if (this->velocidadVector.y > 0) this->animState = PLAYER_ANIMATION_STATES::MOVING_DOWN;
        else this->animState = PLAYER_ANIMATION_STATES::MOVING_UP;
    }

    this->lastPosition = this->getPosition();
}

void Enemigos::attack(Jugador* jugador)
{
    // Base implementation (can be empty or default melee)
}

void Enemigos::update()
{

}

void Enemigos::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);
    target.draw(this->hitbox);
}

//================DUENDE================

Duende::Duende(float x, float y)
        : Enemigos(2, 3, 1, 1, "Duende")
{
    this->tipo = TipoPersonaje::Enemigos;

    this->initTexture("Sprites ejemplo/player_sprites.png");
    this->initSprite();
    this->initHitbox(16.f, 16.f);
    this->hitbox.setScale(this->sprite.getScale());
    this->hitbox.setOrigin(this->sprite.getOrigin().x - 1.5f, this->sprite.getOrigin().y - 3.f);

    this->velocidad = 1.f;
    //this->initAnimations();
    this->setPosition(x, y);
    this->velocidad = 2.5f; // Fast
}

Duende::~Duende()
{

}

void Duende::updateIA(Jugador* jugador)
{
    sf::Vector2f direction = jugador->getPosition() - this->sprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance != 0) {
        direction = direction / distance;
    }

    float SIGHT_RANGE = 100.f;
    float ATTACK_RANGE = 20.f;

    if (distance < ATTACK_RANGE)
    {
        this->attack(jugador);
    }
    else if (distance < SIGHT_RANGE)
    {
        this->velocidadVector.x = direction.x;
        this->velocidadVector.y = direction.y;

        //Animacion
        if (direction.x > 0)
            this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
        else
            this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
    }
    else
    {
        this->animState = PLAYER_ANIMATION_STATES::IDLE;
        this->velocidadVector.x = 0.f;
        this->velocidadVector.y = 0.f;
    }
}

void Duende::update()
{
    //this->updateAnimations();
}

void Duende::attack(Jugador* jugador)
{
    this->animState = PLAYER_ANIMATION_STATES::ATTACK;

    //logica de ataque
    if (this->animationTimer.getElapsedTime().asSeconds() >= 1.f)
    {
        std::cout << "ataca enemigo" << std::endl;
        jugador->recibirDanio(1);
        this->animationTimer.restart();
    }
}

//================ORCO================

Orco::Orco(float x, float y)
    : Enemigos(3, 10, 3, 1, "Orco")
{
    //TODO: Crear sprites
    this->initTexture("Sprites ejemplo/player_sprites.png");
    this->initSprite();
    this->sprite.setColor(sf::Color::Red);
    this->sprite.setScale(3.f, 3.f);
    this->initHitbox(30.f, 30.f);
    this->hitbox.setScale(this->sprite.getScale());
    this->hitbox.setOrigin(this->sprite.getOrigin().x - 1.5f, this->sprite.getOrigin().y - 3.f);

    this->setPosition(x, y);
    this->velocidad = 1.0f; // Slow
}

Orco::~Orco() {}

void Orco::update() {
    //this->updateAnimations();
}

void Orco::updateIA(Jugador* jugador)
{
    // Orco: Perseguir al jugador (Lento pero constante)
    sf::Vector2f direction = jugador->getPosition() - this->sprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance != 0) {
        direction = direction / distance;
    }

    float ATTACK_RANGE = 40.f; // Rango melee un poco mayor por ser grande

    if (distance < ATTACK_RANGE)
    {
        this->attack(jugador);
    }
    else
    {
        this->velocidadVector = direction; // Se mueve hacia el jugador
        
        // Animacion
        if (std::abs(direction.x) > std::abs(direction.y)) {
            if (direction.x > 0) this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
            else this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
        } else {
            if (direction.y > 0) this->animState = PLAYER_ANIMATION_STATES::MOVING_DOWN;
            else this->animState = PLAYER_ANIMATION_STATES::MOVING_UP;
        }
    }
}

void Orco::attack(Jugador* jugador)
{
    this->animState = PLAYER_ANIMATION_STATES::ATTACK;
    this->velocidadVector = sf::Vector2f(0,0);

    if (this->animationTimer.getElapsedTime().asSeconds() >= 1.5f) // Ataque lento
    {
        std::cout << "Orco aplasta!" << std::endl;
        jugador->recibirDanio(this->dmg);
        this->animationTimer.restart();
    }
}

//================HADA================

Hada::Hada(float x, float y)
    : Enemigos(4, 2, 0, 1, "Hada")
{
    this->initTexture("Sprites ejemplo/player_sprites.png");
    this->initSprite();
    this->sprite.setColor(sf::Color::Cyan);
    this->initHitbox(14.f, 14.f);
    this->hitbox.setScale(this->sprite.getScale());
    this->hitbox.setOrigin(this->sprite.getOrigin().x - 1.5f, this->sprite.getOrigin().y - 3.f);

    this->setPosition(x, y);
    this->velocidad = 3.0f;
}

Hada::~Hada() {}

void Hada::update() {
    //this->updateAnimations();
}

void Hada::curarAliados(std::vector<Enemigos*>& enemigos) {
    // Curar aliados cercanos cada 3 segundos
    static sf::Clock healTimer;
    if (healTimer.getElapsedTime().asSeconds() > 3.0f) {
        for (auto* enemigo : enemigos) {
            if (enemigo != this && enemigo->getHp() > 0) {
                float dist = std::sqrt(std::pow(enemigo->getPosition().x - this->getPosition().x, 2) +
                                     std::pow(enemigo->getPosition().y - this->getPosition().y, 2));
                if (dist < 150.f) { // Rango de curacion
                    // No tenemos metodo curar, asi que accedemos directo o asumimos logica
                    // Por ahora solo print, idealmente
                    std::cout << "Hada cura a " << enemigo->getPosition().x << std::endl;
                    //hada->heal(1)
                }
            }
        }
        healTimer.restart();
    }
}

void Hada::updateIA(Jugador* jugador)
{
    // Hada: Huir del jugador
    sf::Vector2f direction = this->sprite.getPosition() - jugador->getPosition(); // Vector opuesto
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance != 0) {
        direction = direction / distance;
    }

    float SAFE_DISTANCE = 200.f;

    if (distance < SAFE_DISTANCE)
    {
        this->velocidadVector = direction; // Huir
    }
    else
    {
        // Deambular o quedarse quieta
        this->velocidadVector = sf::Vector2f(0,0);
        this->animState = PLAYER_ANIMATION_STATES::IDLE;
    }
    
    // Animacion basica
    if (this->velocidadVector.x != 0 || this->velocidadVector.y != 0) {
         if (std::abs(this->velocidadVector.x) > std::abs(this->velocidadVector.y)) {
            if (this->velocidadVector.x > 0) this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
            else this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
        }
    }
}

//================ESTATUA================

Estatua::Estatua(float x, float y)
    : Enemigos(5, 999, 2, 1, "Estatua")
{
    this->initTexture("Sprites ejemplo/player_sprites.png");
    this->initSprite();
    this->sprite.setColor(sf::Color(100, 100, 100));
    this->initHitbox(20.f, 20.f);
    this->hitbox.setScale(this->sprite.getScale());
    this->hitbox.setOrigin(this->sprite.getOrigin().x - 1.5f, this->sprite.getOrigin().y - 3.f);

    this->setPosition(x, y);
    this->velocidad = 1.5f;
}

Estatua::~Estatua() {}

void Estatua::update() {
    //this->updateAnimations();
}

void Estatua::updateIA(Jugador* jugador)
{
    // Estatua: No hace nada, solo existe.
    this->velocidadVector = sf::Vector2f(0,0);
    this->animState = PLAYER_ANIMATION_STATES::IDLE;
    
    // Si el jugador la toca, quizas daño por contacto?
    // Por ahora inerte.
}

bool Estatua::checkDeathCondition(int numEnemies) {
    // Muere si es el unico enemigo o si solo hay estatuas
    // If numEnemies <= num statues
    return false; // Logica en Habitacion
}

//================HECHICERO================

Hechicero::Hechicero(float x, float y)
    : Enemigos(6, 4, 2, 1, "Hechicero")
{
    this->initTexture("Sprites ejemplo/player_sprites.png");
    this->initSprite();
    this->sprite.setColor(sf::Color::Magenta);
    this->initHitbox(16.f, 16.f);
    this->hitbox.setScale(this->sprite.getScale());
    this->hitbox.setOrigin(this->sprite.getOrigin().x - 1.5f, this->sprite.getOrigin().y - 3.f);

    this->setPosition(x, y);
    this->velocidad = 1.8f;
}

Hechicero::~Hechicero() {}

void Hechicero::update() {
    //this->updateAnimations();
}

void Hechicero::attack(Jugador* jugador)
{
    // Melee attack or empty
}

void Hechicero::attack(Jugador* jugador, std::vector<Proyectil*>& proyectiles) {
    if(this->attackTimer.getElapsedTime().asSeconds() > 2.0f) {
        sf::Vector2f dir = jugador->getPosition() - this->getPosition();
        float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
        if(len != 0) dir /= len;
        
        // Spawn proyectil
        proyectiles.push_back(new Proyectil(this->getPosition().x, this->getPosition().y, dir, 4.f, this->dmg, true));
        
        this->attackTimer.restart();
    }
}

void Hechicero::updateIA(Jugador* jugador)
{
    // Hechicero: Mantener distancia
    sf::Vector2f diff = jugador->getPosition() - this->getPosition();
    float dist = std::sqrt(diff.x*diff.x + diff.y*diff.y);
    sf::Vector2f dir = (dist != 0) ? (diff / dist) : sf::Vector2f(0,0);

    float OPTIMAL_DIST = 250.f;
    float TOLERANCE = 50.f;

    if (dist < OPTIMAL_DIST - TOLERANCE)
    {
        this->velocidadVector = -dir; // Alejarse
    }
    else if (dist > OPTIMAL_DIST + TOLERANCE)
    {
        this->velocidadVector = dir; // Acercarse
    }
    else
    {
        this->velocidadVector = sf::Vector2f(0,0); // Mantener posicion
    }
    
    // Animacion
    if (this->velocidadVector.x != 0 || this->velocidadVector.y != 0) {
         if (std::abs(this->velocidadVector.x) > std::abs(this->velocidadVector.y)) {
            if (this->velocidadVector.x > 0) this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
            else this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
        }
    } else {
        this->animState = PLAYER_ANIMATION_STATES::IDLE;
    }
}
