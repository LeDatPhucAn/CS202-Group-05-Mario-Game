#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "raylib.h"
#include "raylib-tileson.h"
#include "json.hpp"
#include "tileson.hpp"
     // Tileson API: tson::Tileson, tson::Map

class MyMap {
public:
    MyMap() = default;

    ~MyMap() {
        // Giải phóng tài nguyên raylib-tileson khi kết thúc
        UnloadMap(drawMap);
    }

    // Load map mới từ filePath; ném runtime_error khi có lỗi
    void choose(const std::string &filePath) {
        // 1) Nếu trước đó đã load một map, unload nó đi
        UnloadMap(drawMap);

        // 2) Load để vẽ (Raylib-Tileson)
        drawMap = LoadTiled(filePath.c_str());
        if (!IsTiledReady(drawMap)) {
            throw std::runtime_error("Failed to load Tiled map for drawing: " + filePath);
        }

        // 3) Parse để lấy tson::Map* (Tileson)
        tson::Tileson parser;
        auto result = parser.parse(filePath);
        if (!result) {
            throw std::runtime_error("Failed to parse Tiled JSON: " + filePath);
        }
        tsonMap = result.get();

        // 4) Lưu lại filePath (nếu cần tái load hoặc debug)
        this->filePath = filePath;
    }

    // Vẽ map ra màn hình tại (x, y)
    void display(int x = 0, int y = 0) const {
        DrawTiled(drawMap, x, y, WHITE);
    }

    // Truy xuất con trỏ tson::Map để lấy dữ liệu layer/object
    tson::Map* data() const {
        return tsonMap;
    }

    Vector2 GetStartingPoint();
    Vector2 GetFinishingPoint();

private:
    std::string filePath;  // để debug hoặc reload
    Map         drawMap{}; // Raylib-Tileson Map
    tson::Map  *tsonMap{}; // Tileson Map pointer
};