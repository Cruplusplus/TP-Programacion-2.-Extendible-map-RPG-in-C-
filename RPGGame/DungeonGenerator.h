#pragma once
#include <vector>
#include <stack>
#include <ctime>
#include <cstdlib>
#include <iostream>

enum RoomType {
    EMPTY = 0,
    START,
    NORMAL,
    TREASURE,
    BOSS
};

struct RoomData {
    int x, y;
    RoomType type;
    bool doors[4]; // 0: Top, 1: Right, 2: Bottom, 3: Left
    bool visited;

    RoomData() {
        x = 0; y = 0;
        type = EMPTY;
        for(int i = 0; i < 4; i++) doors[i] = false;
        visited = false;
    }
};

class DungeonGenerator {
private:
    int width;
    int height;
    std::vector<std::vector<RoomData>> grid;
    int numRooms;

public:
    DungeonGenerator(int w, int h, int nRooms);
    ~DungeonGenerator();

    void generate(unsigned int seed);
    RoomData getRoom(int x, int y);
    int getWidth() const;
    int getHeight() const;
};
