#pragma once
#include "json.hpp"
#include <fstream>
#include <string>
#include <iostream>

using json = nlohmann::json;

enum MENU_MAIN_OPITONS {NEW_GAME = 0, LOAD_GAME, OPTIONS, EXIT};
enum MENU_LOAD_OPITONS {LOAD_GAME_1 = 0, LOAD_GAME_2, LOAD_GAME_3};
enum MENU_SAVE_OPITONS {SAVE_GAME_1 = 0, SAVE_GAME_2, SAVE_GAME_3};
enum MENU_OPTIONS_OPITONS {RES_DEF = 0, RES_1024, FULLSCREEN};
const int BACK = 4;

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
