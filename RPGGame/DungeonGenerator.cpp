#include "DungeonGenerator.h"

DungeonGenerator::DungeonGenerator(int w, int h, int nRooms) {
    this->width = w;
    this->height = h;
    this->numRooms = nRooms;
    this->grid.resize(width, std::vector<RoomData>(height));
}

DungeonGenerator::~DungeonGenerator() {
}

void DungeonGenerator::generate(unsigned int seed) {
    std::srand(seed);

    // Reset grid
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            this->grid[x][y] = RoomData();
            this->grid[x][y].x = x;
            this->grid[x][y].y = y;
        }
    }

    int startX = width / 2;
    int startY = height / 2;
    
    std::vector<std::pair<int, int>> createdRooms;
    createdRooms.push_back({startX, startY});
    this->grid[startX][startY].type = START;

    int currentRooms = 1;
    
    // Loop de creacion de habitaciones
    while(currentRooms < numRooms) {
        // Agarra una habitacion random de las ya creadas
        int randIndex = std::rand() % createdRooms.size();
        int cx = createdRooms[randIndex].first;
        int cy = createdRooms[randIndex].second;

        // Elige una direccion random
        int dir = std::rand() % 4;
        int nx = cx, ny = cy;

        if(dir == 0) ny--; // Top
        else if(dir == 1) nx++; // Right
        else if(dir == 2) ny++; // Bottom
        else if(dir == 3) nx--; // Left

        // Chequea que la habitacion no salga del mapa y que no haya una habitacion en la direccion
        if(nx >= 0 && nx < width && ny >= 0 && ny < height) {
            if(this->grid[nx][ny].type == EMPTY) {
                this->grid[nx][ny].type = NORMAL;
                createdRooms.push_back({nx, ny});
                currentRooms++;
            }
        }
    }

    // Asignacion de habitaciones especiales
    // Busca habitaciones con solo 1 vecino para Boss/Tesoros
    std::vector<std::pair<int, int>> leafRooms;
    for(auto p : createdRooms) {
        if(p.first == startX && p.second == startY) continue; // Skip start

        int neighbors = 0;
        if(p.first > 0 && this->grid[p.first-1][p.second].type != EMPTY) neighbors++;
        if(p.first < width-1 && this->grid[p.first+1][p.second].type != EMPTY) neighbors++;
        if(p.second > 0 && this->grid[p.first][p.second-1].type != EMPTY) neighbors++;
        if(p.second < height-1 && this->grid[p.first][p.second+1].type != EMPTY) neighbors++;
        
        if(neighbors == 1) leafRooms.push_back(p);
    }

    // Fallback si no hay habitaciones con solo 1 vecino (es posible)
    if(leafRooms.empty()) {
        for(auto p : createdRooms) {
             if(p.first == startX && p.second == startY) continue;
             leafRooms.push_back(p);
        }
    }

    // Asignacion de Boss
    if(!leafRooms.empty()) {
        int idx = std::rand() % leafRooms.size();
        this->grid[leafRooms[idx].first][leafRooms[idx].second].type = BOSS;
        leafRooms.erase(leafRooms.begin() + idx);
    }

    // Asignacion de Tesoro
    if(!leafRooms.empty()) {
        int idx = std::rand() % leafRooms.size();
        this->grid[leafRooms[idx].first][leafRooms[idx].second].type = TREASURE;
    } else if (createdRooms.size() > 2) {
         // Elije random non-start non-boss
         for(auto p : createdRooms) {
             if(this->grid[p.first][p.second].type == NORMAL) {
                 this->grid[p.first][p.second].type = TREASURE;
                 break;
             }
         }
    }

    // Calculo de puertas (Coherence Pass)
    for(auto p : createdRooms) {
        int x = p.first;
        int y = p.second;

        // Top
        if(y > 0 && this->grid[x][y-1].type != EMPTY) this->grid[x][y].doors[0] = true;
        // Right
        if(x < width-1 && this->grid[x+1][y].type != EMPTY) this->grid[x][y].doors[1] = true;
        // Bottom
        if(y < height-1 && this->grid[x][y+1].type != EMPTY) this->grid[x][y].doors[2] = true;
        // Left
        if(x > 0 && this->grid[x-1][y].type != EMPTY) this->grid[x][y].doors[3] = true;
    }
}

RoomData DungeonGenerator::getRoom(int x, int y) {
    if(x >= 0 && x < width && y >= 0 && y < height) {
        return this->grid[x][y];
    }
    return RoomData();
}

int DungeonGenerator::getWidth() const { return width; }
int DungeonGenerator::getHeight() const { return height; }
