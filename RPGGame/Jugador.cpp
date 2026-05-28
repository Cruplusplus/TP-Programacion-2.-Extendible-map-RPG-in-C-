#include "Jugador.h"
#include <cmath>
#include <algorithm>

//================JUGADOR================{

void Jugador::initInventory() {
  this->inventory = std::vector<ItemType>();
  this->coins = 0;
  this->statsUp = 0; 
  this->maxHp = 10;
  this->lvl = 1;

  this->levelPiso = 0;
  this->pendingProjectileSpawn = false;
  this->pendingExplosionSpawn = false;
}

Jugador::Jugador(const float x, const float y)
    : Character(1, 10, 1, 1, "Knight") {
  TipoPersonaje::Jugador;
  this->initTexture("Sprites ejemplo/player_sprites.png");
  this->initSprite();

  this->initHitbox(16.f, 16.f);
  this->hitbox.setScale(this->sprite.getScale());
  this->hitbox.setOrigin(this->sprite.getOrigin().x - 1.5f,
                         this->sprite.getOrigin().y - 3.f);

  this->initInventory();

  this->initAnimations();
  this->velocidad = 3.f;

  this->facingDirection = 0;
  this->isBlocking = false;

  this->keybinds["UP"] = sf::Keyboard::W;
  this->keybinds["DOWN"] = sf::Keyboard::S;
  this->keybinds["LEFT"] = sf::Keyboard::A;
  this->keybinds["RIGHT"] = sf::Keyboard::D;
  this->keybinds["DASH"] = sf::Keyboard::Space;
  this->keybinds["ATTACK"] = sf::Keyboard::K;
  this->keybinds["BLOCK"] = sf::Keyboard::L;

  this->setPosition(x, y);
}

const bool Jugador::getAnimSwitch() {
  bool animSwitch = this->animationSwitch;

  if (this->animationSwitch) {
    this->animationSwitch = false;
  }
  return animSwitch;
}

void Jugador::recibirDanio(int danio) { 
    if (!this->isBlocking) {
        this->hp -= danio;
    }
}
int Jugador::getHp() const { return this->hp; }

void Jugador::resetAnimTimer() {
  if (this->animState == PLAYER_ANIMATION_STATES::ATTACK || this->animState == PLAYER_ANIMATION_STATES::BLOCK) return;
  this->animationTimer.restart();
  this->animationSwitch = true;
}

//================MOVIMIENTO==================

void Jugador::recalculateStatsFromItems() {
    this->velocidad = 3.0f; // Base speed
    for (auto item : this->inventory) {
        if (item == ITEM_BOW || item == ITEM_KAMIKAZE || item == ITEM_ARCOMIKAZE) {
            this->velocidad = 4.5f;
        }
    }
}

void Jugador::addItem(ItemType item) {
  bool hasBow = hasItem(ITEM_BOW);
  bool hasKamikaze = hasItem(ITEM_KAMIKAZE);

  if ((item == ITEM_BOW && hasKamikaze) || (item == ITEM_KAMIKAZE && hasBow)) {
    this->inventory.push_back(ITEM_ARCOMIKAZE);
    this->inventory.erase(std::remove(this->inventory.begin(), this->inventory.end(), ITEM_BOW), this->inventory.end());
    this->inventory.erase(std::remove(this->inventory.begin(), this->inventory.end(), ITEM_KAMIKAZE), this->inventory.end());
  } else {
    this->inventory.push_back(item);
  }

  this->recalculateStatsFromItems();
}

bool Jugador::hasItem(ItemType item) const {
  for (auto i : inventory)
    if (i == item)
      return true;
  return false;
}

void Jugador::addPickup(PickupType pickup) {
  switch (pickup) {
  case PICKUP_HEART:
    if (hp < maxHp)
      hp++;
    break;
  case PICKUP_SPECTRAL_HEART:
    hp++; // Puede exceder maxHp
    break;
  case PICKUP_COIN:
    coins++;
    break;
  case PICKUP_ITEM_BOW:
    addItem(ITEM_BOW);
    break;
  case PICKUP_ITEM_KAMIKAZE:
    addItem(ITEM_KAMIKAZE);
    break;
  case PICKUP_ITEM_DASH:
    addItem(ITEM_DASH);
    break;
  case PICKUP_STAT_UP:
    statsUp++;

    //beta random stat up
    int randomNum = rand()%3+1;
    switch(randomNum){
      case 1:
        this->dmg = this->dmg+1;
        std::cout << "DMG up" << std::endl;
        break;
      case 2:
        this->maxHp = this->maxHp+1;
        this->hp = this->hp+1;
        std::cout << "HP up" << std::endl;
        break;
      case 3:
        this->velocidad = this->velocidad+0.5;
        std::cout << "VEL up" << std::endl;
        break;
      default:
        std::cout << "error at Jugador.cpp Jugador::addPickup()";
        break;
    }

    break;
  }
}

//================MOVIMIENTO==================

void Jugador::updateMovement() {
  // Lockea el movimiento durante el ataque
  if (this->animState == PLAYER_ANIMATION_STATES::ATTACK) {
    this->velocidadVector.x = 0.f;
    this->velocidadVector.y = 0.f;
    return;
  }
  
  // Logica de Bloqueo
  if (this->isBlocking) {
      if (this->blockDuration.getElapsedTime().asSeconds() < 0.5f) {
          this->animState = PLAYER_ANIMATION_STATES::BLOCK;
          this->velocidadVector.x = 0.f;
          this->velocidadVector.y = 0.f;
          this->sprite.setColor(sf::Color(100, 150, 255, 230));
          return;
      } else {
          this->isBlocking = false;
          this->sprite.setColor(sf::Color(255, 255, 255, 255));
          this->animState = PLAYER_ANIMATION_STATES::IDLE;
      }
  } else {
      if (sf::Keyboard::isKeyPressed(this->keybinds["BLOCK"]) && !this->isDashing) {
          if (this->blockCooldown.getElapsedTime().asSeconds() > 3.0f) {
              this->isBlocking = true;
              this->blockDuration.restart();
              this->blockCooldown.restart();
              this->animState = PLAYER_ANIMATION_STATES::BLOCK;
              return;
          }
      }
  }

  // Dash Logic
  if (this->isDashing) {
    if (this->dashTimer.getElapsedTime().asSeconds() < 0.2f) {
      this->velocidadVector = this->dashDir * 5.0f;
      return;
    } else {
      this->isDashing = false;
    }
  }

  sf::Vector2f inputDir(0.f, 0.f);
  if (sf::Keyboard::isKeyPressed(this->keybinds["LEFT"]))  inputDir.x -= 1.f;
  if (sf::Keyboard::isKeyPressed(this->keybinds["RIGHT"])) inputDir.x += 1.f;
  if (sf::Keyboard::isKeyPressed(this->keybinds["UP"]))    inputDir.y -= 1.f;
  if (sf::Keyboard::isKeyPressed(this->keybinds["DOWN"]))  inputDir.y += 1.f;

  // Active Items Input (DASH)
  if (sf::Keyboard::isKeyPressed(this->keybinds["DASH"]) && !this->isDashing && hasItem(ITEM_DASH)) {
    if (this->dashTimer.getElapsedTime().asSeconds() > 7.0f) {
      this->isDashing = true;
      this->dashCooldown.restart();
      this->dashTimer.restart();

      this->dashDir = inputDir;

      if (this->dashDir.x == 0.f && this->dashDir.y == 0.f) {
        if (this->facingDirection == DIRECTION::RIGHT) this->dashDir.x = 1.f;
        else if (this->facingDirection == DIRECTION::LEFT) this->dashDir.x = -1.f;
        else if (this->facingDirection == DIRECTION::UP) this->dashDir.y = -1.f;
        else if (this->facingDirection == DIRECTION::DOWN) this->dashDir.y = 1.f;
      } else {
        float length = std::sqrt(this->dashDir.x * this->dashDir.x + this->dashDir.y * this->dashDir.y);
        this->dashDir.x /= length;
        this->dashDir.y /= length;
      }
    }
  }

  this->velocidadVector = inputDir;

  if (this->velocidadVector.x != 0.f || this->velocidadVector.y != 0.f) {
      float length = std::sqrt(std::pow(this->velocidadVector.x, 2) + std::pow(this->velocidadVector.y, 2));
      this->velocidadVector.x /= length;
      this->velocidadVector.y /= length;

      if (std::abs(this->velocidadVector.x) > std::abs(this->velocidadVector.y)) {
          if (this->velocidadVector.x > 0) {
              this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
              this->facingDirection = DIRECTION::RIGHT;
          } else {
              this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
              this->facingDirection = DIRECTION::LEFT;
          }
      } else {
          if (this->velocidadVector.y > 0) {
              this->animState = PLAYER_ANIMATION_STATES::MOVING_DOWN;
              this->facingDirection = DIRECTION::DOWN;
          } else {
              this->animState = PLAYER_ANIMATION_STATES::MOVING_UP;
              this->facingDirection = DIRECTION::UP;
          }
      }
  } else {
      this->animState = PLAYER_ANIMATION_STATES::IDLE;
  }

  if (sf::Keyboard::isKeyPressed(this->keybinds["ATTACK"])) {
    if (hasItem(ITEM_BOW) || hasItem(ITEM_ARCOMIKAZE)) {
      // Bow / Arcomikaze attack (cooldown of 0.4s)
      if (this->bowCooldown.getElapsedTime().asSeconds() > 0.4f) {
        this->animState = PLAYER_ANIMATION_STATES::ATTACK;
        this->resetAttack();
        this->bowCooldown.restart();
        this->pendingProjectileSpawn = true;
      }
    } else if (hasItem(ITEM_KAMIKAZE)) {
      // Kamikaze explosion (cooldown of 3.0s)
      if (this->kamikazeTimer.getElapsedTime().asSeconds() > 3.0f) {
        this->animState = PLAYER_ANIMATION_STATES::ATTACK;
        this->resetAttack();
        this->kamikazeTimer.restart();
        this->pendingExplosionSpawn = true;
      }
    } else {
      // Standard melee attack
      this->animState = PLAYER_ANIMATION_STATES::ATTACK;
      this->resetAttack();
    }
  }
}



//================ANIMACIONES==================

void Jugador::updateAnimations() {
  if (this->animState == PLAYER_ANIMATION_STATES::ATTACK) {
    // Esperar 0.1s antes de mostrar el frame de ataque (windup)
    if (this->animationTimer.getElapsedTime().asSeconds() >= 0.1f) {
      this->currentFrame.width = 49;
      this->currentFrame.height = 36;
      this->currentFrame.top = 143;
      this->currentFrame.left = 20;
      this->sprite.setTextureRect(this->currentFrame);
    }

    // Terminar el ataque después de un tiempo total (ej. 0.5s)
    if (this->animationTimer.getElapsedTime().asSeconds() >= 0.2f) {
      this->animState = PLAYER_ANIMATION_STATES::IDLE;
      this->resetAttack(); // Limpiar lista de enemigos golpeados

      // Opcional: Resetear tamaño del frame inmediatamente para evitar glitches
      // visuales
      this->currentFrame.width = 19;
      this->currentFrame.height = 44;
      this->currentFrame.top = 45;
      this->currentFrame.left = 20;
    }

    return;
  }

  if (this->animState == PLAYER_ANIMATION_STATES::IDLE) {
    if (this->animationTimer.getElapsedTime().asSeconds() >= 0.2f ||
        getAnimSwitch()) {
      this->currentFrame.top = 45;
      this->currentFrame.left = 20;
      this->currentFrame.width = 19;
      this->currentFrame.height = 44;

      this->sprite.setScale(2.5f, 2.5f);
      this->sprite.setOrigin(0.f, 0.f);

      this->animationTimer.restart();
      this->sprite.setTextureRect(this->currentFrame);
    }

    return;
  }

  if (this->animState == PLAYER_ANIMATION_STATES::MOVING_RIGHT) {
    if (this->animationTimer.getElapsedTime().asSeconds() >= 0.2f ||
        getAnimSwitch()) {
      this->currentFrame.top = 0;
      this->currentFrame.left += 20;
      this->currentFrame.width = 19;
      this->currentFrame.height = 44;
      if (this->currentFrame.left >= 80.f)
        this->currentFrame.left = 0;

      this->sprite.setScale(2.5f, 2.5f);
      this->sprite.setOrigin(0.f, 0.f);

      this->animationTimer.restart();
      this->sprite.setTextureRect(this->currentFrame);
    }

    return;
  }

  if (this->animState == PLAYER_ANIMATION_STATES::MOVING_LEFT) {
    if (this->animationTimer.getElapsedTime().asSeconds() >= 0.2f ||
        getAnimSwitch()) {
      this->currentFrame.width = 19;
      this->currentFrame.height = 44;
      this->currentFrame.top = 0;
      this->currentFrame.left += 20;
      if (this->currentFrame.left >= 80.f)
        this->currentFrame.left = 0;

      this->sprite.setScale(-2.5f, 2.5f);
      this->sprite.setOrigin(19.f, 0.f);

      this->animationTimer.restart();
      this->sprite.setTextureRect(this->currentFrame);
    }

    return;
  }

  if (this->animState == PLAYER_ANIMATION_STATES::MOVING_UP) {

    if (this->animationTimer.getElapsedTime().asSeconds() >= 0.2f ||
        getAnimSwitch()) {
      this->currentFrame.top = 89;
      this->currentFrame.left += 20;
      this->currentFrame.width = 19;
      this->currentFrame.height = 44;
      if (this->currentFrame.left >= 78.f)
        this->currentFrame.left = 0.f;

      this->sprite.setScale(2.5f, 2.5f);
      this->sprite.setOrigin(0.f, 0.f);

      this->animationTimer.restart();
      this->sprite.setTextureRect(this->currentFrame);
    }

    return;
  }

  if (this->animState == PLAYER_ANIMATION_STATES::MOVING_DOWN) {
    if (this->animationTimer.getElapsedTime().asSeconds() >= 0.2f ||
        getAnimSwitch()) {
      this->currentFrame.top = 45;
      this->currentFrame.left += 20;
      this->currentFrame.width = 19;
      this->currentFrame.height = 44;
      if (this->currentFrame.left >= 78.f)
        this->currentFrame.left = 0.f;

      this->sprite.setScale(2.5f, 2.5f);
      this->sprite.setOrigin(0.f, 0.f);

      this->animationTimer.restart();
      this->sprite.setTextureRect(this->currentFrame);
    }

    return;
  }
}

//=============UPDATE Y RENDER=============

void Jugador::update() {
  this->updateMovement();
  this->updateAnimations();

  // Debug Hitbox Logic
  if (this->isAttacking()) {
    sf::FloatRect ahb = this->getAttackHitbox();
    this->debugHb.setSize(sf::Vector2f(ahb.width, ahb.height));
    this->debugHb.setPosition(ahb.left, ahb.top);
    this->debugHb.setFillColor(
        sf::Color(255, 0, 0, 100)); // Rojo semitransparente
    this->debugHb.setOutlineColor(sf::Color::Red);
    this->debugHb.setOutlineThickness(1.f);
  } else {
    this->debugHb.setFillColor(sf::Color::Transparent);
    this->debugHb.setOutlineColor(sf::Color::Transparent);
  }
}

void Jugador::render(sf::RenderTarget &target) {
  target.draw(this->sprite);
  target.draw(this->debugHb);

  // Kamikaze visual feedback
  if (this->isAttacking() && this->hasItem(ITEM_KAMIKAZE) && !this->hasItem(ITEM_BOW)) {
    sf::FloatRect bounds = this->getAttackHitbox();
    sf::CircleShape explosion(bounds.width / 2.f);
    explosion.setPosition(bounds.left, bounds.top);
    explosion.setFillColor(sf::Color(255, 128, 0, 100)); // Transparent orange
    explosion.setOutlineColor(sf::Color(255, 0, 0, 180));
    explosion.setOutlineThickness(2.f);
    target.draw(explosion);
  }
}

// Getters & Setters
std::vector<int> Jugador::getInventoryAsInt() const {
  std::vector<int> inv;
  for (auto i : inventory)
    inv.push_back((int)i);
  return inv;
}

int Jugador::getCoins() const { return this->coins; }
int Jugador::getStatsUp() const { return this->statsUp; }
int Jugador::getDmg() const { return this->dmg; }
int Jugador::getMaxHp() const { return this->maxHp; }

int Jugador::getLevelPiso() const { return this->levelPiso; }
void Jugador::setLevelPiso(int piso) { this->levelPiso = piso; }
void Jugador::addLevelPiso() { this->levelPiso++; }

void Jugador::setStats(int hp, int maxHp, int coins, int dmg,
                       const std::vector<int> &inv) {
  this->hp = hp;
  this->maxHp = maxHp;
  this->coins = coins;
  this->dmg = dmg;
  this->inventory.clear();
  for (int i : inv) {
    this->inventory.push_back((ItemType)i);
  }
  this->recalculateStatsFromItems();
}

void Jugador::resetAttack() { this->hitEnemies.clear(); }
bool Jugador::hasHit(Character *target) {
  for (auto *hit : hitEnemies) {
    if (hit == target)
      return true;
  }
  return false;
}
void Jugador::addHit(Character *target) { this->hitEnemies.push_back(target); }

bool Jugador::isAttacking() const {
  return this->animState == PLAYER_ANIMATION_STATES::ATTACK;
}


sf::FloatRect Jugador::getAttackHitbox() const {
  if (hasItem(ITEM_BOW) || hasItem(ITEM_ARCOMIKAZE)) {
    return sf::FloatRect(0, 0, 0, 0); // No melee attack if shooting arrows
  }

  if (hasItem(ITEM_KAMIKAZE)) {
    sf::FloatRect playerBounds = this->getHitboxBounds();
    float size = 200.f; // explosion range
    return sf::FloatRect(playerBounds.left + playerBounds.width/2.f - size/2.f,
                         playerBounds.top + playerBounds.height/2.f - size/2.f,
                         size, size);
  }

  sf::FloatRect swordHb = this->getHitboxBounds();
  swordHb.height -= 90.f;
  float range = 90.f; // Rango de la espada
  float width = 50.f; // Ancho de la espada

  sf::FloatRect playerHb = this->getHitboxBounds();
  playerHb.width += range;
  playerHb.height += range;

  if (facingDirection == DIRECTION::DOWN)
  {
    swordHb.height += 90.f;
    return sf::FloatRect(swordHb.left + swordHb.width / 2 - width / 2,
                         swordHb.top + swordHb.height, width, range);
  } else if (facingDirection == DIRECTION::LEFT)
  {
    return sf::FloatRect(swordHb.left - range,
                         swordHb.top + swordHb.height / 2 - width / 2, range,
                         width);
  } else if (facingDirection == DIRECTION::RIGHT)
  {
    return sf::FloatRect(swordHb.left + swordHb.width,
                         swordHb.top + swordHb.height / 2 - width / 2, range,
                         width);
  } else if (facingDirection == DIRECTION::UP)
  {
    return sf::FloatRect(swordHb.left + swordHb.width / 2 - width / 2,
                         swordHb.top - 130.f, width, range);
  }
  return sf::FloatRect(0, 0, 0, 0);
}

bool Jugador::getPendingProjectileSpawn() const {
  return this->pendingProjectileSpawn;
}

void Jugador::resetPendingProjectileSpawn() {
  this->pendingProjectileSpawn = false;
}

bool Jugador::getPendingExplosionSpawn() const {
  return this->pendingExplosionSpawn;
}

void Jugador::resetPendingExplosionSpawn() {
  this->pendingExplosionSpawn = false;
}

int Jugador::getFacingDirection() const {
  return this->facingDirection;
}
