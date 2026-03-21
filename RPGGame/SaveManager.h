#pragma once
#include "json.hpp"
#include <fstream>
#include <string>
#include <iostream>

using json = nlohmann::json;

struct GameData {
    int hp;
    int maxHp;
    int coins;
    int keys;
    unsigned int seed;
    int currentRoomX;
    int currentRoomY;
    std::vector<int> inventory; // Guarda ItemType como int
};

class SaveManager {
public:
    static void saveGame(int slot, const GameData& data);
    static GameData loadGame(int slot);
    static bool saveExists(int slot);
    
    static void saveConfig(int width, int height, bool fullscreen);
    static void loadConfig(int& width, int& height, bool& fullscreen);
};
