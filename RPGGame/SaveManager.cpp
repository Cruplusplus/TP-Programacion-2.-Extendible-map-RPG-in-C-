#include "SaveManager.h"

void SaveManager::saveGame(int slot, const GameData& data) {
    json j;
    j["hp"] = data.hp;
    j["maxHp"] = data.maxHp;
    j["coins"] = data.coins;
    j["keys"] = data.keys;
    j["seed"] = data.seed;
    j["currentRoomX"] = data.currentRoomX;
    j["currentRoomY"] = data.currentRoomY;
    j["inventory"] = data.inventory;

    std::string filename = "Save_Knight_" + std::to_string(slot) + ".json";
    std::ofstream file(filename);
    if(file.is_open()) {
        file << j.dump(4);
        file.close();
        std::cout << "Game saved to " << filename << std::endl;
    } else {
        std::cout << "Error saving game to " << filename << std::endl;
    }
}

GameData SaveManager::loadGame(int slot) {
    GameData data;
    std::string filename = "Save_Knight_" + std::to_string(slot) + ".json";
    std::ifstream file(filename);
    
    if(file.is_open()) {
        json j;
        file >> j;
        
        data.hp = j.value("hp", 10);
        data.maxHp = j.value("maxHp", 10);
        data.coins = j.value("coins", 0);
        data.keys = j.value("keys", 0);
        data.seed = j.value("seed", 0);
        data.currentRoomX = j.value("currentRoomX", 0);
        data.currentRoomY = j.value("currentRoomY", 0);
        
        if(j.contains("inventory")) {
            data.inventory = j["inventory"].get<std::vector<int>>();
        }
        
        file.close();
    } else {
        std::cout << "Error loading game from " << filename << std::endl;
        // Return default/empty data
        data.hp = 10;
        data.maxHp = 10;
        data.coins = 0;
        data.keys = 0;
        data.seed = 0;
        data.currentRoomX = 0;
        data.currentRoomY = 0;
    }
    
    return data;
}

bool SaveManager::saveExists(int slot) {
    std::string filename = "Save_Knight_" + std::to_string(slot) + ".json";
    std::ifstream file(filename);
    return file.good();
}

void SaveManager::saveConfig(int width, int height, bool fullscreen) {
    json j;
    j["width"] = width;
    j["height"] = height;
    j["fullscreen"] = fullscreen;
    
    std::ofstream file("Visual_Config.json");
    if(file.is_open()) {
        file << j.dump(4);
        file.close();
    }
}

void SaveManager::loadConfig(int& width, int& height, bool& fullscreen) {
    std::ifstream file("Visual_Config.json");
    if(file.is_open()) {
        json j;
        file >> j;
        width = j.value("width", 800);
        height = j.value("height", 600);
        fullscreen = j.value("fullscreen", false);
        file.close();
    } else {
        // Default
        width = 800;
        height = 600;
        fullscreen = false;
    }
}
