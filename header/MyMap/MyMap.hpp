#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <filesystem>

#include "raylib.h"
#include "tileson.hpp"
#include "Block.hpp"

class MyMap {
    friend class Game;
public:
    MyMap() = default;
    ~MyMap() { clearAll(); }

    // Load JSON, cache tileset textures, then create Block* per layer
    void choose(const std::string &jsonPath);

    // Draw: Image → Tiles → Objects
    void display(int ox = 0, int oy = 0) const;
    void update();
    
    Rectangle getMapBounds();

    std::vector<Block*> imageBlocks;
    std::vector<Block*> tileBlocks;
    std::vector<Block*> objectBlocks;

private:
    Rectangle mapBounds;
    tson::Map* tsonMap = nullptr;
    std::unordered_map<int, Texture2D> tilesetCache;

    void clearAll();

    static void drawBlock(const Block* b, int ox, int oy);
};
