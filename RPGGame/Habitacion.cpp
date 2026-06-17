#include "Habitacion.h"
#include "Enemigos.h"
#include "Jugador.h"
#include <cmath>

Habitacion::Habitacion(sf::Texture *tile_sheet, RoomData data, int nivelPiso) {
  this->tileSheet = tile_sheet;
  this->pisoActual = nivelPiso;
  this->tileMap = nullptr;
  this->roomData = data;

  if (!this->backgroundTexture.loadFromFile(
          "Sprites ejemplo/suelo_habitacion.png")) {
    std::cout << "ERROR: No se pudo cargar el fondo de la habitacion"
              << std::endl;
  }
  this->backgroundSprite.setTexture(this->backgroundTexture);
  this->backgroundSprite.setScale(3.2f, 3.2f);
  this->backgroundSprite.setPosition(5, 15);

  this->trapdoorShape.setSize(sf::Vector2f(50.f, 50.f));
  this->trapdoorShape.setFillColor(sf::Color(20, 20, 20));
  this->trapdoorShape.setOutlineColor(sf::Color(100, 100, 100));
  this->trapdoorShape.setOutlineThickness(3.f);
  this->trapdoorShape.setPosition(375.f, 275.f); //center

  this->initTileMap();
  this->initEnemigos();
}

Habitacion::~Habitacion() {
  delete this->tileMap;

  for (auto *enemigo : this->enemigos) {
    delete enemigo;
  }

  for (auto *pickup : this->pickups) {
    delete pickup;
  }

  for (auto *p : this->proyectiles) {
    delete p;
  }
}

#include "RoomTemplates.h"

void Habitacion::initTileMap() {
  const unsigned TILE_SIZE = 50;
  const int MAP_WIDTH = 16;  // 16 * 50 = 800
  const int MAP_HEIGHT = 12; // 12 * 50 = 600

  int levelData[MAP_HEIGHT][MAP_WIDTH];
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      levelData[y][x] = 0;
    }
  }

  std::array<std::string_view, MAP_HEIGHT> selectedTemplate;

  if (roomData.type != BOSS && roomData.type != START &&
      roomData.type != TREASURE) {
    int tIndex = rand() % RoomTemplates::templates.size();
    selectedTemplate = RoomTemplates::templates[tIndex];
  } else {
    for (int i = 0; i < MAP_HEIGHT; i++)
      selectedTemplate[i] = "................";
  }

  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      char c = selectedTemplate[y][x];

      if (c == 'R')
        levelData[y][x] = tipoTiles::ROCA;
      else if (c == 'E') {
        this->enemySpawns.push_back(sf::Vector2f(
            x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2));
      }
    }
  }

  for (int x = 0; x < MAP_WIDTH; x++) {
    levelData[0][x] = 3;
    levelData[MAP_HEIGHT - 1][x] = 3;
  }
  for (int y = 0; y < MAP_HEIGHT; y++) {
    levelData[y][0] = 3;
    levelData[y][MAP_WIDTH - 1] = 3;
  }

  levelData[0][0] = 4;
  levelData[0][MAP_WIDTH - 1] = 4;
  levelData[MAP_HEIGHT - 1][0] = 4;
  levelData[MAP_HEIGHT - 1][MAP_WIDTH - 1] = 4;

  if (roomData.doors[0])
  {
    levelData[0][MAP_WIDTH / 2] = 0;
    levelData[0][MAP_WIDTH / 2 - 1] = 0;
  }
  if (roomData.doors[1])
  {
    levelData[MAP_HEIGHT / 2][MAP_WIDTH - 1] = 0;
    levelData[MAP_HEIGHT / 2 - 1][MAP_WIDTH - 1] = 0;
  }
  if (roomData.doors[2])
  {
    levelData[MAP_HEIGHT - 1][MAP_WIDTH / 2] = 0;
    levelData[MAP_HEIGHT - 1][MAP_WIDTH / 2 - 1] = 0;
  }
  if (roomData.doors[3])
  {
    levelData[MAP_HEIGHT / 2][0] = 0;
    levelData[MAP_HEIGHT / 2 - 1][0] = 0;
  }

  this->tileMap =
      new TileMap(MAP_WIDTH, MAP_HEIGHT, this->tileSheet, TILE_SIZE);

  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      int tileType = levelData[y][x];
      if (tileType != 0) {
        this->tileMap->addTile(x, y, tileType);
      }
    }
  }
}

void Habitacion::initEnemigos() {
  if (this->roomData.type == START)
    return;

  if (this->roomData.type == BOSS) {
    // Spawn al jefe (placeholder for now)
    this->enemigos.push_back(
        new Orco(400.f, 300.f)); // Por ahora es un orco pero mas grande
    this->enemigos.back()->getSprite().setScale(4.f, 4.f);
    this->enemigos.back()->escalarDificultad(this->pisoActual);
    return;
  }

  if (this->roomData.type == TREASURE) {
    this->pickups.push_back(new Pickup(PICKUP_ITEM_BOW, 400.f, 300.f));
    this->pickups.push_back(new Pickup(PICKUP_ITEM_KAMIKAZE, 450.f, 300.f));
    this->pickups.push_back(new Pickup(PICKUP_ITEM_DASH, 350.f, 300.f));
    return;
  }

  for (auto &pos : this->enemySpawns) {
    int type = rand() % 100;
    Enemigos* e = nullptr;

    if (type < 40)
      e = new Duende(pos.x, pos.y);
    else if (type < 60)
      e = new Orco(pos.x, pos.y);
    else if (type < 75)
      e = new Hada(pos.x, pos.y);
    else if (type < 90)
      e = new Hechicero(pos.x, pos.y);
    else
      e = new Estatua(pos.x, pos.y);
      
    e->escalarDificultad(this->pisoActual);
    this->enemigos.push_back(e);
  }
}

void Habitacion::triggerExplosion(sf::Vector2f pos, float damage, float radius) {
    for (auto* e : this->enemigos) {
        float dx = e->getPosition().x - pos.x;
        float dy = e->getPosition().y - pos.y;
        float dist = std::sqrt(dx*dx + dy*dy);
        if (dist <= radius) {
            e->recibirDanio(damage);
        }
    }
    
    VisualExplosion exp;
    exp.pos = pos;
    exp.timer.restart();
    exp.maxRadius = radius;
    this->visualExplosions.push_back(exp);
}


void Habitacion::update(Jugador *jugador) {
  this->tileMap->update();

  if (jugador->getPendingProjectileSpawn()) {
      sf::Vector2f dir(0.f, 0.f);
      int facing = jugador->getFacingDirection();
      if (facing == 0) dir.y = 1.f;
      else if (facing == 1) dir.x = -1.f;
      else if (facing == 2) dir.x = 1.f;
      else if (facing == 3) dir.y = -1.f;
      
      sf::FloatRect pb = jugador->getHitboxBounds();
      float px = pb.left + pb.width/2.f - 5.f;
      float py = pb.top + pb.height/2.f - 5.f;
      
      Proyectil* proj = new Proyectil(px, py, dir, 10.f, jugador->getDmg(), false);
      if (jugador->hasItem(ITEM_ARCOMIKAZE)) {
          proj->getSprite().setColor(sf::Color(255, 100, 0));
          proj->getSprite().setScale(1.5f, 1.5f);
      }
      this->proyectiles.push_back(proj);
      jugador->resetPendingProjectileSpawn();
  }

  if (jugador->getPendingExplosionSpawn()) {
      sf::FloatRect pb = jugador->getHitboxBounds();
      sf::Vector2f center(pb.left + pb.width/2.f, pb.top + pb.height/2.f);
      
      VisualExplosion exp;
      exp.pos = center;
      exp.timer.restart();
      exp.maxRadius = 100.f;
      this->visualExplosions.push_back(exp);
      
      jugador->resetPendingExplosionSpawn();
  }

  for (size_t i = 0; i < this->visualExplosions.size(); i++) {
      if (this->visualExplosions[i].timer.getElapsedTime().asSeconds() > 0.25f) {
          this->visualExplosions.erase(this->visualExplosions.begin() + i);
          i--;
      }
  }

  //debug pickups
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T))
  {
    int xrand = rand() % 200 + 300;
    int yrand = rand() % 200 + 200;
    this->pickups.push_back(new Pickup(PICKUP_STAT_UP, xrand, yrand));
  }

  int statuesCount = 0;
  int otherEnemiesCount = 0;

  bool playerAttacking = jugador->isAttacking();
  sf::FloatRect attackHb;
  if (playerAttacking)
    attackHb = jugador->getAttackHitbox();

  for (auto *enemigo : this->enemigos) {
    if (dynamic_cast<Estatua *>(enemigo))
      statuesCount++;
    else
      otherEnemiesCount++;

    enemigo->updateIA(jugador);

    if (playerAttacking) {
      if (attackHb.intersects(enemigo->getSprite().getGlobalBounds())) {
        if (!jugador->hasHit(enemigo)) {
          enemigo->recibirDanio(jugador->getDmg());
          jugador->addHit(enemigo);
        }
      }
    }

    if (Hada *hada = dynamic_cast<Hada *>(enemigo)) {
      hada->curarAliados(this->enemigos);
    }
    if (Hechicero *hechicero = dynamic_cast<Hechicero *>(enemigo)) {
      hechicero->attack(jugador, this->proyectiles);
    }

    enemigo->update();
  }

  if (otherEnemiesCount == 0 && statuesCount > 0) {
    for (size_t i = 0; i < this->enemigos.size(); i++) {
      if (dynamic_cast<Estatua *>(this->enemigos[i])) {
        this->enemigos[i]->recibirDanio(9999);
      }
    }
  }

  for (size_t i = 0; i < this->enemigos.size(); i++) {
    if (this->enemigos[i]->getHp() <= 0) {
      if (rand() % 100 < 30) {
        PickupType pt = (PickupType)(rand() % 4);
        this->pickups.push_back(new Pickup(pt,
                                           this->enemigos[i]->getPosition().x,
                                           this->enemigos[i]->getPosition().y));
      }

      delete this->enemigos[i];
      this->enemigos.erase(this->enemigos.begin() + i);
      i--;
    }
  }

  for (size_t i = 0; i < this->pickups.size(); i++) {
    if (this->pickups[i]->getGlobalBounds().intersects(
            jugador->getHitboxBounds())) {
      jugador->addPickup(this->pickups[i]->type);
      delete this->pickups[i];
      this->pickups.erase(this->pickups.begin() + i);
      i--;
    }
  }

  for (size_t i = 0; i < this->proyectiles.size(); i++) {
      this->proyectiles[i]->update();

      if (this->proyectiles[i]->isEnemy()) {
          if (this->proyectiles[i]->getGlobalBounds().intersects(jugador->getHitboxBounds())) {
              jugador->recibirDanio(this->proyectiles[i]->getDamage());
              delete this->proyectiles[i];
              this->proyectiles.erase(this->proyectiles.begin() + i);
              i--;
              continue;
          }
      }

      if (!this->proyectiles[i]->isEnemy()) {
          bool hit = false;
          for (auto* enemigo : this->enemigos) {
              if (this->proyectiles[i]->getGlobalBounds().intersects(enemigo->getSprite().getGlobalBounds())) {
                  hit = true;
                  
                  if (jugador->hasItem(ITEM_ARCOMIKAZE)) {
                      this->triggerExplosion(this->proyectiles[i]->getPosition(), this->proyectiles[i]->getDamage() * 2, 100.f);
                  } else {
                      enemigo->recibirDanio(this->proyectiles[i]->getDamage());
                  }
                  break;
              }
          }
          if (hit) {
              delete this->proyectiles[i];
              this->proyectiles.erase(this->proyectiles.begin() + i);
              i--;
              continue;
          }

          if (this->tileMap->checkCollision(this->proyectiles[i]->getGlobalBounds())) {
              if (jugador->hasItem(ITEM_ARCOMIKAZE)) {
                  this->triggerExplosion(this->proyectiles[i]->getPosition(), this->proyectiles[i]->getDamage() * 2, 100.f);
              }
              delete this->proyectiles[i];
              this->proyectiles.erase(this->proyectiles.begin() + i);
              i--;
              continue;
          }
      }

      if (this->proyectiles[i]->getPosition().x < 0 || this->proyectiles[i]->getPosition().x > 800 ||
          this->proyectiles[i]->getPosition().y < 0 || this->proyectiles[i]->getPosition().y > 600) {
          delete this->proyectiles[i];
          this->proyectiles.erase(this->proyectiles.begin() + i);
          i--;
      }
  }
}

void Habitacion::renderFondo(sf::RenderTarget &target) {
  target.draw(this->backgroundSprite);

  this->tileMap->render(target);

  if (this->roomData.type == BOSS && this->enemigos.empty()) {
      target.draw(this->trapdoorShape);
  }

  for (auto *p : this->pickups) {
    p->render(target);
  }

  for (auto *p : this->proyectiles) {
      p->render(target);
  }

  for (const auto& exp : this->visualExplosions) {
      float t = exp.timer.getElapsedTime().asSeconds();
      float currentRadius = exp.maxRadius * (t / 0.25f);
      if (currentRadius > exp.maxRadius) currentRadius = exp.maxRadius;
      
      sf::CircleShape circle(currentRadius);
      circle.setOrigin(currentRadius, currentRadius);
      circle.setPosition(exp.pos);
      
      int alpha = 120 - static_cast<int>(120 * (t / 0.25f));
      if (alpha < 0) alpha = 0;
      int outlineAlpha = 200 - static_cast<int>(200 * (t / 0.25f));
      if (outlineAlpha < 0) outlineAlpha = 0;
      
      circle.setFillColor(sf::Color(255, 100, 0, alpha));
      circle.setOutlineColor(sf::Color(255, 0, 0, outlineAlpha));
      circle.setOutlineThickness(2.f);
      
      target.draw(circle);
  }
}

TileMap *Habitacion::getTileMap() const { return this->tileMap; }
std::vector<Enemigos *> Habitacion::getEnemigos() { return this->enemigos; }
void Habitacion::clearEnemies() {
  for (auto *e : this->enemigos) {
    delete e;
  }
  this->enemigos.clear();
}
RoomData Habitacion::getRoomData() const { return roomData; }

sf::FloatRect Habitacion::getTrapdoorBounds() const {
    if (this->roomData.type == BOSS && this->enemigos.empty()) {
        return this->trapdoorShape.getGlobalBounds();
    }
    return sf::FloatRect(0.f, 0.f, 0.f, 0.f);
}
