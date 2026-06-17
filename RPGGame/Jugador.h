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

    sf::Clock dashTimer;
    sf::Clock dashCooldown;
    sf::Clock kamikazeTimer;
    sf::Clock blockDuration;
    sf::Clock blockCooldown;
    bool isDashing;
    bool isBlocking;
    sf::Vector2f dashDir;
    sf::RectangleShape debugHb;

    int facingDirection;
    std::vector<Character*> hitEnemies;
    
    bool pendingProjectileSpawn;
    bool pendingExplosionSpawn;
    sf::Clock bowCooldown;

    void recalculateStatsFromItems();


public:
    std::map<std::string, sf::Keyboard::Key> keybinds;

    bool getPendingProjectileSpawn() const;
    void resetPendingProjectileSpawn() ;
    bool getPendingExplosionSpawn() const;
    void resetPendingExplosionSpawn();
    int getFacingDirection() const;

    Jugador(const float x, const float y);

    void resetAnimTimer();
    const bool getAnimSwitch();
    
    sf::FloatRect getAttackHitbox() const;
    void resetAttack();
    bool hasHit(Character* target);
    void addHit(Character* target);
    bool isAttacking() const;

    void addItem(ItemType item);
    void addPickup(PickupType pickup);
    bool hasItem(ItemType item) const;

    virtual void recibirDanio(int danio);

    void updateMovement();

    std::vector<int> getInventoryAsInt() const;
    int getHp() const;
    int getCoins() const;
    int getStatsUp() const;
    int getDmg() const;
    int getMaxHp() const;

    int getLevelPiso() const;
    void setLevelPiso(int piso);
    void addLevelPiso();

    void setStats(int hp, int maxHp, int coins, int dmg, const std::vector<int>& inv);
    
    void updateAnimations();
    void update() override;

    void render(sf::RenderTarget& target) override;

};
