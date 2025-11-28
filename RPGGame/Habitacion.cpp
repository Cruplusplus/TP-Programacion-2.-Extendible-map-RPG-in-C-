#include "Habitacion.h"
#include "Enemigos.h"
#include "Jugador.h"

Habitacion::Habitacion(sf::Texture* tile_sheet, RoomData data)
{
    this->tileSheet = tile_sheet;
    this->tileMap = nullptr;
    this->roomData = data;

    if (!this->backgroundTexture.loadFromFile("Sprites ejemplo/suelo_habitacion.png"))
    {
        std::cout << "ERROR: No se pudo cargar el fondo de la habitacion" << std::endl;
    }
    this->backgroundSprite.setTexture(this->backgroundTexture);
    this->backgroundSprite.setScale(3.f, 3.f);
    // this->backgroundSprite.setPosition(x, y);

    this->initTileMap();
    this->initEnemigos();
}

Habitacion::~Habitacion()
{
    delete this->tileMap;

    // Borramos todos los enemigos del vector
    for (auto* enemigo : this->enemigos)
    {
        delete enemigo;
    }
    
    // Borramos todos los pickups
    for (auto* pickup : this->pickups)
    {
        delete pickup;
    }
}

void Habitacion::initTileMap()
{
    const unsigned TILE_SIZE = 50;
    const int MAP_WIDTH = 16; // 16 * 50 = 800
    const int MAP_HEIGHT = 12; // 12 * 50 = 600

    // Inicializar el piso
    int levelData[MAP_HEIGHT][MAP_WIDTH];
    for(int y=0; y<MAP_HEIGHT; y++)
        for(int x=0; x<MAP_WIDTH; x++)
            levelData[y][x] = 0;

    // paredes
    for(int x=0; x<MAP_WIDTH; x++) {
        levelData[0][x] = 3; // Top
        levelData[MAP_HEIGHT-1][x] = 3; // Bottom
    }
    for(int y=0; y<MAP_HEIGHT; y++) {
        levelData[y][0] = 3; // Left
        levelData[y][MAP_WIDTH-1] = 3; // Right
    }

    // Esquinas
    levelData[0][0] = 4;
    levelData[0][MAP_WIDTH-1] = 4;
    levelData[MAP_HEIGHT-1][0] = 4;
    levelData[MAP_HEIGHT-1][MAP_WIDTH-1] = 4;

    // puertas (Crear aberturas si existe puerta)
    if(roomData.doors[0]) { // Top
        levelData[0][MAP_WIDTH/2] = 0;
        levelData[0][MAP_WIDTH/2 - 1] = 0;
    }
    if(roomData.doors[1]) { // Right
        levelData[MAP_HEIGHT/2][MAP_WIDTH-1] = 0;
        levelData[MAP_HEIGHT/2 - 1][MAP_WIDTH-1] = 0;
    }
    if(roomData.doors[2]) { // Bottom
        levelData[MAP_HEIGHT-1][MAP_WIDTH/2] = 0;
        levelData[MAP_HEIGHT-1][MAP_WIDTH/2 - 1] = 0;
    }
    if(roomData.doors[3]) { // Left
        levelData[MAP_HEIGHT/2][0] = 0;
        levelData[MAP_HEIGHT/2 - 1][0] = 0;
    }

    // Obstaculos aleatorios (rocas) dentro
    if(roomData.type != BOSS && roomData.type != START) {
        for(int i=0; i<10; i++) {
            int rx = rand() % (MAP_WIDTH - 2) + 1;
            int ry = rand() % (MAP_HEIGHT - 2) + 1;
            if(levelData[ry][rx] == 0) levelData[ry][rx] = 5; // Rock
        }
    }

    this->tileMap = new TileMap(MAP_WIDTH, MAP_HEIGHT, this->tileSheet, TILE_SIZE);

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            int tileType = levelData[y][x];
            if (tileType != 0)
            {
                this->tileMap->addTile(x, y, tileType);
            }
        }
    }
}

void Habitacion::initEnemigos()
{
    if(this->roomData.type == START) return;

    if(this->roomData.type == BOSS) {
        // Spawn al jefe (placeholder for now)
        this->enemigos.push_back(new Orco(400.f, 300.f)); // Por ahora es un orco pero mas grande
        this->enemigos.back()->getSprite().setScale(4.f, 4.f);
        return;
    }

    if(this->roomData.type == TREASURE) {
        // Spawn items (representados como pickup por ahora)
        // Por simplicidad, solo se generan monedas y corazones
        // TODO: Implementar drops de items correctamente. Por ahora, solo monedas y corazones.
        this->pickups.push_back(new Pickup(PICKUP_COIN, 400.f, 300.f));
        this->pickups.push_back(new Pickup(PICKUP_KEY, 450.f, 300.f));
        this->pickups.push_back(new Pickup(PICKUP_SPECTRAL_HEART, 350.f, 300.f));
        return;
    }

    // Random enemies
    int numEnemies = rand() % 4 + 2;
    for(int i=0; i<numEnemies; i++) {
        float ex = float(rand() % 600 + 100); // 100 to 700 (Width 800)
        float ey = float(rand() % 400 + 100); // 100 to 500 (Height 600)

        int type = rand() % 100;
        if(type < 40) // 40% Duende
            this->enemigos.push_back(new Duende(ex, ey));
        else if(type < 60) // 20% Orco
            this->enemigos.push_back(new Orco(ex, ey));
        else if(type < 75) // 15% Hada
            this->enemigos.push_back(new Hada(ex, ey));
        else if(type < 90) // 15% Hechicero
            this->enemigos.push_back(new Hechicero(ex, ey));
        else // 10% Estatua
            this->enemigos.push_back(new Estatua(ex, ey));
    }
}

void Habitacion::update(Jugador* jugador)
{
    this->tileMap->update();

    int statuesCount = 0;
    int otherEnemiesCount = 0;
    
    bool playerAttacking = jugador->isAttacking();
    sf::FloatRect attackHb;
    if(playerAttacking) attackHb = jugador->getAttackHitbox();

    for (auto* enemigo : this->enemigos)
    {
        if(dynamic_cast<Estatua*>(enemigo)) statuesCount++;
        else otherEnemiesCount++;

        enemigo->updateIA(jugador);
        
        // Player Attack Collision
        if(playerAttacking) {
            if(attackHb.intersects(enemigo->getHitboxBounds())) {
                if(!jugador->hasHit(enemigo)) {
                    enemigo->recibirDanio(1); // Base dmg
                    jugador->addHit(enemigo);
                    std::cout << "Golpe direccional AOE!" << std::endl;
                }
            }
        }

        // Special updates
        if(Hada* hada = dynamic_cast<Hada*>(enemigo)) {
            //logica de curar enemigos
        }
        if(Hechicero* hechicero = dynamic_cast<Hechicero*>(enemigo)) {
            hechicero->attack(jugador);
        }

        //animaciones y etc
        enemigo->update();
    }

    // Condicion de la estatua
    if(otherEnemiesCount == 0 && statuesCount > 0) {
        for (size_t i = 0; i < this->enemigos.size(); i++) {
            if(dynamic_cast<Estatua*>(this->enemigos[i])) {
                this->enemigos[i]->recibirDanio(9999); // mata la estatua
            }
        }
    }

    // Remueve enemigos muertos
    for (size_t i = 0; i < this->enemigos.size(); i++) {
        if(this->enemigos[i]->getHp() <= 0) {
            // Chance de drop de pickup
            if(rand() % 100 < 30) { // 30% chance
                PickupType pt = (PickupType)(rand() % 4);
                this->pickups.push_back(new Pickup(pt, this->enemigos[i]->getPosition().x, this->enemigos[i]->getPosition().y));
            }

            delete this->enemigos[i];
            this->enemigos.erase(this->enemigos.begin() + i);
            i--;
        }
    }

    // Update Pickups
    for (size_t i = 0; i < this->pickups.size(); i++) {
        if(this->pickups[i]->getGlobalBounds().intersects(jugador->getHitboxBounds())) {
            jugador->addPickup(this->pickups[i]->type);
            delete this->pickups[i];
            this->pickups.erase(this->pickups.begin() + i);
            i--;
        }
    }
}

void Habitacion::renderFondo(sf::RenderTarget& target)
{
    target.draw(this->backgroundSprite);

    this->tileMap->render(target);

    for(auto* p : this->pickups) {
        p->render(target);
    }
}

TileMap* Habitacion::getTileMap() const { return this->tileMap; }
std::vector<Enemigos*> Habitacion::getEnemigos() { return this->enemigos; }
RoomData Habitacion::getRoomData() const { return roomData; }
