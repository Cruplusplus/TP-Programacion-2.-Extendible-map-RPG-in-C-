#pragma once
#include "Character.h"
#include "Items.h"
#include <map>
#include <string>

enum DIRECTION {DOWN = 0, LEFT, RIGHT, UP};

class Jugador : public Character
{
private:
    std::vector<ItemType> inventory;
    int coins;
    int statsUp;
    int maxHp;

    int levelPiso;

    void initInventory();

    // Cooldowns
    sf::Clock dashTimer;
    sf::Clock dashCooldown;
    sf::Clock kamikazeTimer;
    sf::Clock blockDuration;
    sf::Clock blockCooldown;
    bool isDashing;
    bool isBlocking;
    sf::Vector2f dashDir;
    sf::RectangleShape debugHb;
    // Attack Logic
    int facingDirection; // 0: Down, 1: Left, 2: Right, 3: Up
    std::vector<Character*> hitEnemies;

public:
    std::map<std::string, sf::Keyboard::Key> keybinds;

    Jugador(const float x, const float y);

    //setters
    void resetAnimTimer();
    const bool getAnimSwitch();
    
    // Attack
    sf::FloatRect getAttackHitbox() const;
    void resetAttack();
    bool hasHit(Character* target);
    void addHit(Character* target);
    bool isAttacking() const;

    
    // Items & stats
    void addItem(ItemType item);
    void addPickup(PickupType pickup);
    bool hasItem(ItemType item);

    virtual void recibirDanio(int danio);

    int getHp() const;

    void updateMovement();

    std::vector<int> getInventoryAsInt() const;
    int getCoins() const;
    int getStatsUp() const;
    int getDmg() const;
    int getMaxHp() const;

    int getLevelPiso() const;
    void addLevelPiso();

    void setStats(int hp, int maxHp, int coins, int statsUp, const std::vector<int>& inv);
    
    void updateAnimations();
    void update() override;

    void render(sf::RenderTarget& target) override;

};
