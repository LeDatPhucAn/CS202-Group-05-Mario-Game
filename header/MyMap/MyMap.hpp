#pragma once

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <filesystem>

#include "raylib.h"
#include "tileson.hpp"
#include "Block.hpp"

class MyMap {
    friend class Game;
public:
    MyMap();
    ~MyMap();

    // Load JSON, cache tileset textures, then create Block* per layer
    void choose(const string &jsonPath);

    // Draw: Image → Tiles → Objects
    void display(int ox = 0, int oy = 0) const;
    void update();
    Rectangle getMapBounds() const;
    Vector2 StartingPoint;
    Vector2 FinishingPoint;

private:
    // Parsing and setup steps
    bool parseJson(const string &jsonPath);
    void cacheTilesets();
    void setupMapBounds();
    void scanLayers();

    // Layer handlers
    void handleImageLayer(const tson::Layer &layer);
    void handleTileLayer(const tson::Layer &layer,
                         const map<uint32_t, tson::Tile*> &tileMap,
                         int mapW);
    void handleObjectLayer(tson::Layer &layer);

    // Helpers
    const TSInfo* selectTSInfo(int gid) const;
    Rectangle calcSrcRect(const TSInfo &info, int gid) const;
    void clearAll();

    // Data members
    tson::Map* tsonMap;
    filesystem::path baseDir;
    unordered_map<int, Texture2D> tilesetCache;
    vector<TSInfo> tsinfo;
    Rectangle mapBounds;
    vector<Block*> imageBlocks;
    vector<Block*> tileBlocks;
    vector<Block*> objectBlocks;


   
};

