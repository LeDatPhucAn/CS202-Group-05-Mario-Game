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
public:
    MyMap() = default;
    ~MyMap() {
        clearAll();
    }

    // Load JSON, cache tileset textures, rồi tạo Block* theo từng layer
    void choose(const std::string &jsonPath) {
        clearAll();

        // 1) Parse JSON bằng Tileson
        tson::Tileson parser;
        auto parsed = parser.parse(jsonPath);
        if (!parsed || parsed->getStatus() != tson::ParseStatus::OK) return;
        tsonMap = parsed.release();

        // 2) Cache textures của từng tileset
        struct TSInfo { int firstgid, columns; Vector2 tileSize; };
        std::vector<TSInfo> tsinfo;
        auto baseDir = std::filesystem::path(jsonPath).parent_path();
        for (auto &ts : tsonMap->getTilesets()) {
            auto imgRel = ts.getImage();
            std::string path = (baseDir / imgRel).string();
            Texture2D tex = LoadTexture(path.c_str());
            tilesetCache[ts.getFirstgid()] = tex;
            int cols = tex.width / ts.getTileSize().x;
            tsinfo.push_back({ ts.getFirstgid(), cols,
                               { float(ts.getTileSize().x), float(ts.getTileSize().y) } });
        }
        std::sort(tsinfo.begin(), tsinfo.end(),
                  [](auto &a, auto &b){ return a.firstgid < b.firstgid; });

        // 3) Lấy kích thước map
        int mapW  = tsonMap->getSize().x;
        int mapH  = tsonMap->getSize().y;
        int tileW = tsonMap->getTileSize().x;
        int tileH = tsonMap->getTileSize().y;

        // 4) Scan tất cả layer
        for (auto &layer : tsonMap->getLayers()) {
            switch (layer.getType()) {
                case tson::LayerType::ImageLayer: {
                    // background image
                    auto imgRel = layer.getImage();
                    std::string path = (baseDir / imgRel).string();
                    Texture2D tex = LoadTexture(path.c_str());
                    Rectangle src{ 0,0,float(tex.width),float(tex.height) };
                    Vector2 pos{ layer.getOffset().x, layer.getOffset().y };
                    imageBlocks.push_back(new Block(0, pos, { float(tex.width), float(tex.height) }, tex, src));
                    break;
                }
                case tson::LayerType::TileLayer: {
                    // grid layer
                    const auto &data = layer.getData();
                    for (int i = 0; i < (int)data.size(); ++i) {
                        int gid = data[i];
                        if (!gid) continue;
                        // tìm tileset
                        const TSInfo* tsi = nullptr;
                        for (int j = tsinfo.size()-1; j >= 0; --j)
                            if (gid >= tsinfo[j].firstgid) { tsi = &tsinfo[j]; break; }
                        if (!tsi) continue;
                        int local = gid - tsi->firstgid;
                        int col   = local % tsi->columns;
                        int row   = local / tsi->columns;
                        Rectangle src{
                            col * tsi->tileSize.x,
                            row * tsi->tileSize.y,
                            tsi->tileSize.x,
                            tsi->tileSize.y
                        };
                        int x = (i % mapW) * tileW;
                        int y = (i / mapW) * tileH;
                        tileBlocks.push_back(new Block(
                            gid,
                            { float(x), float(y) },
                            { float(tileW), float(tileH) },
                            tilesetCache[tsi->firstgid],
                            src
                        ));
                    }
                    break;
                }
                case tson::LayerType::ObjectGroup: {
                    // object layer (Question, Breakable,…)
                    for (auto &obj : layer.getObjects()) {
                        int gid = obj.getGid();
                        if (!gid) continue;
                        const TSInfo* tsi = nullptr;
                        for (int j = tsinfo.size()-1; j >= 0; --j)
                            if (gid >= tsinfo[j].firstgid) { tsi = &tsinfo[j]; break; }
                        if (!tsi) continue;
                        int local = gid - tsi->firstgid;
                        int col   = local % tsi->columns;
                        int row   = local / tsi->columns;
                        Rectangle src{
                            col * tsi->tileSize.x,
                            row * tsi->tileSize.y,
                            tsi->tileSize.x,
                            tsi->tileSize.y
                        };
                        Vector2 pos{
                            obj.getPosition().x,
                            obj.getPosition().y - tsi->tileSize.y
                        };
                        // Constructor tự động đọc properties
                        objectBlocks.push_back(new Block(
                            obj, pos, tsi->tileSize,
                            tilesetCache[tsi->firstgid],
                            src
                        ));
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }

    // Vẽ theo thứ tự: Image → Tile → Object
    void display(int ox = 0, int oy = 0) const {
        for (auto *b : imageBlocks) drawBlock(b, ox, oy);
        for (auto *b : tileBlocks)  drawBlock(b, ox, oy);
        for (auto *b : objectBlocks)drawBlock(b, ox, oy);
    }

    // Chỉ update objectBlocks (có logic / animation)
    void update(float dt) {
        for (auto *b : objectBlocks) b->update(dt);
    }

private:
    tson::Map* tsonMap = nullptr;

    std::vector<Block*> imageBlocks;
    std::vector<Block*> tileBlocks;
    std::vector<Block*> objectBlocks;

    std::unordered_map<int, Texture2D> tilesetCache;

    void clearAll() {
        for (auto *b : imageBlocks)  delete b;
        for (auto *b : tileBlocks)   delete b;
        for (auto *b : objectBlocks) delete b;
        imageBlocks.clear();
        tileBlocks.clear();
        objectBlocks.clear();
        for (auto &kv : tilesetCache) UnloadTexture(kv.second);
        tilesetCache.clear();
    }

    static void drawBlock(const Block* b, int ox, int oy) {
        Vector2 p = b->getPosition();
        DrawTextureRec(b->texture, b->srcRec, { p.x + ox, p.y + oy }, WHITE);
    }
};
