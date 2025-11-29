#pragma once
#include "Character.h"
#include "Items.h"

class Jugador : public Character
{
private:
    std::vector<ItemType> inventory;
    int coins;
    int keys;
    int maxHp;

    void initInventory();

    // Cooldowns
    sf::Clock dashTimer;
    sf::Clock kamikazeTimer;
    bool isDashing;
    sf::Vector2f dashDir;
    sf::RectangleShape debugHb;
    // Attack Logic
    int facingDirection; // 0: Down, 1: Left, 2: Right, 3: Up
    std::vector<Character*> hitEnemies;

public:
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
    int getFacingDirection() const;
    
    // Items & stats
    void addItem(ItemType item);
    void addPickup(PickupType pickup);
    bool hasItem(ItemType item);

    virtual void recibirDanio(int danio);
    int getDmg() const;
    int getHp() const;

    void updateMovement();

    std::vector<int> getInventoryAsInt() const;
    int getCoins() const;
    int getKeys() const;
    int getMaxHp() const;

    void setStats(int hp, int maxHp, int coins, int keys, const std::vector<int>& inv);

    void atacar(Character *enemigo);
    void updateAnimations();
    void update() override;

    void render(sf::RenderTarget& target) override;

};
