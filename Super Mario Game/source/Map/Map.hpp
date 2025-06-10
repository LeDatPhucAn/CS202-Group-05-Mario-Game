// Map.hpp
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <filesystem>

#include "raylib.h"
#include "json.hpp"
using json = nlohmann::json;
using namespace std;
/*
  ===================================================================
   CLASS Tile
   - Đóng gói 1 ô (tile) duy nhất để vẽ: id (GID), vùng nguồn trên
     spritesheet và vị trí vẽ trên màn hình.
  ===================================================================
*/
class Tile {
public:
    // Khởi tạo tile với GID và vùng lấy ảnh
    Tile(int id, const Rectangle& src)
        : _id(id), _srcRect(src), _pos{0,0}
    {}

    // Gán vị trí pixel trên màn hình
    void setPosition(const Vector2& pos) {
        _pos = pos;
    }

    // Vẽ tile bằng DrawTextureRec (cắt từ texture theo srcRect)
    void draw(Texture2D& texture) const {
        DrawTextureRec(texture, _srcRect, _pos, WHITE);
    }

private:
    int       _id;       // Global ID (theo JSON), để tra ngược về TileSet gốc
    Rectangle _srcRect;  // Vùng cắt trên spritesheet, chứa hình của tile
    Vector2   _pos;      // Vị trí vẽ (pixel), được SetPostion gắn vào
};


/*
  ===================================================================
   CLASS Tileset
   - Chứa metadata và texture của 1 tập tileset.
   - Khi loadFromJson, sẽ:
     + đọc các thông số firstGid, tileWidth/Height, columns, tileCount…
     + load texture từ đường dẫn JSON
     + sinh vector tileRects[i] = Rectangle cắt cho mỗi tile
  ===================================================================
*/
class Tileset {
public:
    Tileset() = default;
    ~Tileset();

    // Disable copy
    Tileset(const Tileset&) = delete;
    Tileset& operator=(const Tileset&) = delete;

    // Enable move
    Tileset(Tileset&& other) noexcept;
    Tileset& operator=(Tileset&& other) noexcept;

    void loadFromJson(const json& tsJson);
    Rectangle getSrcRect(int gid) const;
    Texture2D& getTexture() { return _texture; }
    const Texture2D& getTexture() const { return _texture; }
    int getFirstGid() const { return _firstGid; }
    int getTileCount() const { return _tileCount; }

private:
    // Giải phóng texture khi hủy
    void unload();

    int        _firstGid;    // GID đầu tiên trong tập này, dùng để so sánh khi tìm tileset cho GID bất kỳ
    std::string _name;       // Tên tileset (từ JSON)
    Texture2D  _texture;     // Texture chứa spritesheet
    int        _tileWidth;   // Chiều rộng mỗi tile (px) trong spritesheet
    int        _tileHeight;  // Chiều cao mỗi tile (px) trong spritesheet
    int        _columns, _tileCount;     // Số cột trên spritesheet, cần cho việc tính cắt ở đâu spritesheet, Tổng số tile trong spritesheet
    std::vector<Rectangle> _tileRects; // Mảng các Rect để cắt tile ra khỏi spritesheet
};


/*
  ===================================================================
   CLASS Layer
   - Lưu layout của 1 tile-layer:
     + name, kích thước width×height (số ô)
     + data[]: vector GID length = width*height
     + visible, opacity để điều khiển hiển thị
   - draw(ts): duyệt toàn bộ data và vẽ tile tương ứng trên ts
  ===================================================================
*/
class Layer {
public:

    friend class Map;

    Layer() = default;

    // Parse JSON của layer (tilelayer) từ map JSON
    void loadFromJson(const json& layerJson);

    // Trả về GID tại ô (x,y)
    int getTileGid(int x, int y) const;

    // Thay đổi GID tại ô (x,y) (nếu cần runtime)
    void setTileGid(int x, int y, int gid);

    // Vẽ toàn bộ layer: duyệt data, tìm srcRect và draw
    void draw(const std::vector<Tileset>& tilesets,
              int globalTileWidth, int globalTileHeight) const;

private:
    std::string _name;
    int         _width;    // số cột của layer (214)
    int         _height;   // số hàng của layer (16)
    std::vector<int> _data;// length = width*height
    bool        _visible;
    float       _opacity;
};


/*
  ===================================================================
   CLASS Map
   - Là container chính:
     + width, height: kích thước map (theo ô)
     + tileWidth, tileHeight: kích thước tile chung
     + vector<Tileset> tilesets
     + vector<Layer>   layers
   - loadFromFile: đọc stage1-1.json, ghép tileset & layer
   - draw(): vẽ lần lượt từng layer
  ===================================================================
*/
class Map {
public:
    
    Map() = default;

    // Load JSON map từ file, khởi tạo tilesets + layers
    bool loadFromFile(const std::string& filename);

    // Vẽ toàn bộ map
    void draw() const;

    // Tìm layer theo tên (vd. "Collision")
    Layer* getLayerByName(const std::string& name);

private:
    // Helper: tìm tileset chứa GID
    const Tileset* findTilesetForGid(int gid) const;

    int _width;        // số cột
    int _height;       // số hàng
    int _tileWidth;    // pixel
    int _tileHeight;   // pixel

    std::vector<Tileset> _tilesets;
    std::vector<Layer>   _layers;
};



// #pragma once
// #include<iostream>
// #include<string>
// #include<vector>7
// #include "raylib.h"
// #include <nlohmann/json.hpp>
// using json = nlohmann::json;

// using namespace std;


// class Tile {
// public:
//     Tile(int _id, const Rectangle& _srcRect) : id(_id), srcRect(_srcRect) {};
//     void setPosition(const Vector2& _pos) {
//         pos.x = _pos.x;
//         pos.y = _pos.y;
//     }
//     void draw(Texture2D& tileSetTexture) {
//         DrawTexture(tileSetTexture, pos.x, pos.y, WHITE);
//     }

// private:
//     int id;
//     Rectangle srcRect;
//     Vector2 pos; 
// };

// class Tileset {
// public:
//     void loadFromJson(const json& tsJson);
//     Rectangle getSrcRect(int gid) {

//     };

//     Texture2D& getTexture(){

//     };

// private: 
//     int firstGid;
//     string name;
//     Texture2D texture;
//     int tileWidth, tileHeight;
//     int columns, tileCount;
//     vector<Rectangle> tileRects;
// };

// class Layer {
// public:
//     void loadFromJson(const json& layerJson) {

//     }

// int getTileGid(int x, int y) {

// }

// void setTileGid(int x, int y, int gid) {

// }
// void draw(const Tileset& ts) {

// }
// private:
//     string name;
//     int width, height;
//     vector<int> data;
//     bool visible;
//     float opacity;
// };

// class Map {
// public:
    

// private:
//     int width, height;
//     int tileWidth, tileHeight;
//     vector<Tileset> tilesets;
//     vector<Layer> layers;
// };