#include "Game_MapTiles.h"
#include "Enemigos.h"
#include "DungeonGenerator.h"
#include "Items.h"
#include "Proyectil.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Habitacion
{
private:
    sf::Texture* tileSheet;
    TileMap* tileMap;
    std::vector<Enemigos*> enemigos;
    std::vector<Pickup*> pickups;
    RoomData roomData;
    std::vector<sf::Vector2f> enemySpawns;
    std::vector<Proyectil*> proyectiles;

    struct VisualExplosion {
        sf::Vector2f pos;
        sf::Clock timer;
        float maxRadius;
    };
    std::vector<VisualExplosion> visualExplosions;

    void triggerExplosion(sf::Vector2f pos, float damage, float radius = 100.f);


    //BG
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::RectangleShape trapdoorShape;

    void initTileMap();
    void initEnemigos();

    int pisoActual;
public:
    Habitacion(sf::Texture* tile_sheet, RoomData data, int nivelPiso);
    virtual ~Habitacion();

    void update(Jugador* jugador); // Pasar la pos del jugador para la IA

    void renderFondo(sf::RenderTarget& target);

    TileMap* getTileMap() const;
    std::vector<Enemigos*> getEnemigos();
    void clearEnemies();
    RoomData getRoomData() const;
    sf::FloatRect getTrapdoorBounds() const;
};
