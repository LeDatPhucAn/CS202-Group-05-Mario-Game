
#include "MyMap.hpp"
#include <algorithm>

MyMap::MyMap()
    : tsonMap(nullptr) {}

MyMap::~MyMap() {
    clearAll();
}

void MyMap::choose(const std::string &jsonPath) {
    clearAll();
    baseDir = std::filesystem::path(jsonPath).parent_path();
    if (!parseJson(jsonPath)) return;
    cacheTilesets();
    setupMapBounds();
    scanLayers();
}

void MyMap::display(int ox, int oy) const {
    for (auto *b : imageBlocks)  b->display();
    for (auto *b : tileBlocks)   b->display();
    for (auto *b : objectBlocks) b->display();
}

void MyMap::update() {
    for (auto *b : tileBlocks) b->update();
}

Rectangle MyMap::getMapBounds() const {
    return mapBounds;
}

// Step 1: parse JSON
bool MyMap::parseJson(const std::string &jsonPath) {
    tson::Tileson parser;
    auto parsed = parser.parse(jsonPath);
    if (!parsed || parsed->getStatus() != tson::ParseStatus::OK) {
        return false;
    }
    tsonMap = parsed.release();
    return true;
}

// Step 2: load tileset textures & gather metadata
void MyMap::cacheTilesets() {
    for (auto &ts : tsonMap->getTilesets()) {
        auto imgRel = ts.getImage();
        auto fullPath = (baseDir / imgRel).string();
        Texture2D tex = LoadTexture(fullPath.c_str());

        TSInfo info;
        info.firstgid = ts.getFirstgid();
        info.margin   = ts.getMargin();
        info.spacing  = ts.getSpacing();
        info.tileSize = { float(ts.getTileSize().x), float(ts.getTileSize().y) };
        int tileW = ts.getTileSize().x;
        int tileH = ts.getTileSize().y;
        info.columns = (tex.width - 2*info.margin + info.spacing) / (tileW + info.spacing);

        tilesetCache[info.firstgid] = tex;
        tsinfo.push_back(info);
    }
    std::sort(tsinfo.begin(), tsinfo.end(), [](auto &a, auto &b){
        return a.firstgid < b.firstgid;
    });
}

// Step 3: calculate map bounds
void MyMap::setupMapBounds() {
    int mapW  = tsonMap->getSize().x;
    int mapH  = tsonMap->getSize().y;
    int tileW = tsonMap->getTileSize().x;
    int tileH = tsonMap->getTileSize().y;
    mapBounds = Rectangle{ 0, 0, float(mapW * tileW), float(mapH * tileH) };
}

// Step 4: scan layers
void MyMap::scanLayers() {
    auto &tileMap = tsonMap->getTileMap();
    int mapW = tsonMap->getSize().x;
    for (auto &layer : tsonMap->getLayers()) {
        switch (layer.getType()) {
            case tson::LayerType::ImageLayer:
                handleImageLayer(layer);
                break;
            case tson::LayerType::TileLayer:
                handleTileLayer(layer, tileMap, mapW);
                break;
            case tson::LayerType::ObjectGroup:
                handleObjectLayer(layer);
                break;
            default:
                break;
        }
    }
}

void MyMap::handleImageLayer(const tson::Layer &layer) {
    auto path = (baseDir / layer.getImage()).string();
    Texture2D tex = LoadTexture(path.c_str());
    Rectangle src{ 0, 0, float(tex.width), float(tex.height) };
    Vector2 pos{ layer.getOffset().x, layer.getOffset().y };
    imageBlocks.push_back(new Block(0, pos, { float(tex.width), float(tex.height) }, tex, src));
}

void MyMap::handleTileLayer(const tson::Layer &layer,
                             const std::map<uint32_t, tson::Tile*> &tileMap,
                             int mapW)
{
    const auto &data = layer.getData();
    int tileW = tsonMap->getTileSize().x;
    int tileH = tsonMap->getTileSize().y;

    for (int i = 0; i < (int)data.size(); ++i) {
        uint32_t gid = data[i]; if (gid == 0) continue;
        const TSInfo* info = selectTSInfo(gid);
        if (!info) continue;

        Rectangle src = calcSrcRect(*info, gid);
        Vector2 pos{ float((i % mapW) * tileW), float((i / mapW) * tileH) };
        auto it = tileMap.find(gid);
        
        if (it != tileMap.end()) {
            tileBlocks.push_back(new Block(it->second, pos, { float(tileW), float(tileH) },
                tilesetCache[info->firstgid], src, info));
        } else {
            tileBlocks.push_back(new Block(gid, pos, { float(tileW), float(tileH) },
                tilesetCache[info->firstgid], src));
        }
    }
}

void MyMap::handleObjectLayer(tson::Layer &layer) {
    for (auto &obj : layer.getObjects()) {
        int gid = obj.getGid(); if (gid == 0) continue;
        const TSInfo* info = selectTSInfo(gid); if (!info) continue;
        Rectangle src = calcSrcRect(*info, gid);
        Vector2 pos{ obj.getPosition().x, obj.getPosition().y - info->tileSize.y };
        objectBlocks.push_back(new Block(obj, pos, info->tileSize,
            tilesetCache[info->firstgid], src));
    }
}

const TSInfo* MyMap::selectTSInfo(int gid) const {
    for (int i = (int)tsinfo.size() - 1; i >= 0; --i) {
        if (gid >= tsinfo[i].firstgid) return &tsinfo[i];
    }
    return nullptr;
}

Rectangle MyMap::calcSrcRect(const TSInfo &info, int gid) const {
    int local = gid - info.firstgid;
    int col   = local % info.columns;
    int row   = local / info.columns;
    return Rectangle{
        info.margin + col * (info.tileSize.x + info.spacing),
        info.margin + row * (info.tileSize.y + info.spacing),
        info.tileSize.x,
        info.tileSize.y
    };
}

void MyMap::clearAll() {
    for (auto *b : imageBlocks)  delete b;
    for (auto *b : tileBlocks)   delete b;
    for (auto *b : objectBlocks) delete b;
    imageBlocks.clear(); tileBlocks.clear(); objectBlocks.clear();
    for (auto &kv : tilesetCache) UnloadTexture(kv.second);
    tilesetCache.clear(); tsinfo.clear();
    delete tsonMap; tsonMap = nullptr;
}
