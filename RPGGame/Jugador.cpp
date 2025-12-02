#include "Jugador.h"
#include <cmath>

//================JUGADOR================{

void Jugador::initInventory() {
  this->inventory = std::vector<ItemType>();
  this->coins = 0;
  this->keys = 0;
  this->maxHp = 10;
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
  this->velocidad = 2.f;

  this->facingDirection = 0;

  this->setPosition(x, y);
}

const bool Jugador::getAnimSwitch() {
  bool animSwitch = this->animationSwitch;

  if (this->animationSwitch) {
    this->animationSwitch = false;
  }
  return animSwitch;
}

void Jugador::recibirDanio(int danio) { this->hp -= danio; }
int Jugador::getDmg() const { return this->dmg; }
int Jugador::getHp() const { return this->hp; }

void Jugador::resetAnimTimer() {
  this->animationTimer.restart();
  this->animationSwitch = true;
}

//================MOVIMIENTO==================

void Jugador::addItem(ItemType item) {
  // Check synergy
  bool hasBow = hasItem(ITEM_BOW);
  bool hasKamikaze = hasItem(ITEM_KAMIKAZE);

  if (item == ITEM_BOW && hasKamikaze) {
    inventory.push_back(ITEM_ARCOMIKAZE);
  } else if (item == ITEM_KAMIKAZE && hasBow) {
    inventory.push_back(ITEM_ARCOMIKAZE);
  } else {
    inventory.push_back(item);
  }
}

bool Jugador::hasItem(ItemType item) {
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
  case PICKUP_KEY:
    keys++;
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

  this->velocidadVector.x = 0.f;
  this->velocidadVector.y = 0.f;

  this->animState = PLAYER_ANIMATION_STATES::IDLE;

  // Dash Logic
  if (this->isDashing) {
    if (this->dashTimer.getElapsedTime().asSeconds() < 0.2f) {
      this->velocidadVector = this->dashDir * 5.0f; // Velocidad del dash
      return; // Salta el movimiento normal
    } else {
      this->isDashing = false;
    }
  }

  // Active Items Input
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && hasItem(ITEM_DASH)) {
    if (this->dashTimer.getElapsedTime().asSeconds() > 7.0f) {
      this->isDashing = true;
      this->dashTimer.restart();

      this->dashDir = sf::Vector2f(1.f, 0.f);
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        this->dashDir = sf::Vector2f(-1.f, 0.f);
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        this->dashDir = sf::Vector2f(0.f, -1.f);
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        this->dashDir = sf::Vector2f(0.f, 1.f);
    }
  }

  if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
      !sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
      !sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
      !sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    this->velocidadVector.x = 0.f;
    this->velocidadVector.y = 0.f;
    this->animState = PLAYER_ANIMATION_STATES::IDLE;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    this->velocidadVector.x = -1.f;
    this->animState = PLAYER_ANIMATION_STATES::MOVING_LEFT;
    this->facingDirection = 1;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    this->velocidadVector.x = 1.f;
    this->animState = PLAYER_ANIMATION_STATES::MOVING_RIGHT;
    this->facingDirection = 2;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    this->velocidadVector.y = -1.f;
    this->animState = PLAYER_ANIMATION_STATES::MOVING_UP;
    this->facingDirection = 3;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    this->velocidadVector.y = 1.f;
    this->animState = PLAYER_ANIMATION_STATES::MOVING_DOWN;
    this->facingDirection = 0;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
    if (hasItem(ITEM_KAMIKAZE) || hasItem(ITEM_ARCOMIKAZE)) {
      if (this->kamikazeTimer.getElapsedTime().asSeconds() > 7.0f) {
        this->animState = PLAYER_ANIMATION_STATES::ATTACK;
        this->resetAttack();
        this->kamikazeTimer.restart();
      }
    } else {
      this->animState = PLAYER_ANIMATION_STATES::ATTACK;
      this->resetAttack();
    }
  }
}

void Jugador::atacar(Character *enemigo) {
  // Esta funcion es principalmente para activar el estado de ataque
  // La logica real de daño se maneja en Habitacion::update usando
  // getAttackHitbox

  if (this->animState != PLAYER_ANIMATION_STATES::ATTACK) {
    // Inicia ataque
    this->resetAttack();
    // Reset timer para asegurar que la animacion se reproduzca desde el inicio
    // Pero lo manejamos generalmente en updateAnimations
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
    if (this->animationTimer.getElapsedTime().asSeconds() >= 0.5f) {
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

void Jugador::render(sf::RenderTarget &target) { target.draw(this->sprite); }

// Getters & Setters
std::vector<int> Jugador::getInventoryAsInt() const {
  std::vector<int> inv;
  for (auto i : inventory)
    inv.push_back((int)i);
  return inv;
}

int Jugador::getCoins() const { return this->coins; }
int Jugador::getKeys() const { return this->keys; }
int Jugador::getMaxHp() const { return this->maxHp; }

void Jugador::setStats(int hp, int maxHp, int coins, int keys,
                       const std::vector<int> &inv) {
  this->hp = hp;
  this->maxHp = maxHp;
  this->coins = coins;
  this->keys = keys;
  this->inventory.clear();
  for (int i : inv)
    this->inventory.push_back((ItemType)i);
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
int Jugador::getFacingDirection() const { return this->facingDirection; }

sf::FloatRect Jugador::getAttackHitbox() const {
  sf::FloatRect swordHb = this->getHitboxBounds();
  float range = 75.f; // Rango de la espada
  float width = 50.f; // Ancho de la espada

  sf::FloatRect playerHb = this->getHitboxBounds();
  playerHb.width += range;
  playerHb.height += range;

  if (facingDirection == 0) // Down
  {
    return sf::FloatRect(swordHb.left + swordHb.width / 2 - width / 2,
                         swordHb.top + swordHb.height, width, range);
  } else if (facingDirection == 1) // Left
  {
    return sf::FloatRect(swordHb.left - range,
                         swordHb.top + swordHb.height / 2 - width / 2, range,
                         width);
  } else if (facingDirection == 2) // Right
  {
    return sf::FloatRect(swordHb.left + swordHb.width,
                         swordHb.top + swordHb.height / 2 - width / 2, range,
                         width);
  } else if (facingDirection == 3) // Up
  {
    return sf::FloatRect(swordHb.left + swordHb.width / 2 - width / 2,
                         swordHb.top - range, width, range);
  }
  return sf::FloatRect(0, 0, 0, 0);
}
