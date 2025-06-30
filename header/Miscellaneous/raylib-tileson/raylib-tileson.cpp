#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "json.hpp"   // thư viện JSON
#include <fstream> 
#include "raylib.h"
#include "raylib-tileson.h"
#include "tileson.hpp"


// TODO: Add World support with LoadTiledWorld()

class RaylibTilesonData {
    public:
        std::map<std::string, Texture> textures;
        tson::Map* map;
        std::string   jsonFilePath; 
};

Color ColorFromTiledColor(tson::Colori color) {
    Color output;
    output.r = color.r;
    output.g = color.g;
    output.b = color.b;
    output.a = color.a;
    return output;
}

Rectangle RectangleFromTiledRectangle(tson::Rect rect) {
    Rectangle output;
    output.x = rect.x;
    output.y = rect.y;
    output.width = rect.width;
    output.height = rect.height;
    return output;
}

void LoadTiledImage(RaylibTilesonData* data, const char* baseDir, const std::string& imagepath, tson::Colori transparentColor) {
    // Only load the image if it's not yet loaded.
    if (data->textures.count(imagepath) == 0) {
        const char* image;
        if (TextLength(baseDir) > 0) {
            image = TextFormat("%s/%s", baseDir, imagepath.c_str());
        } else {
            image = imagepath.c_str();
        }

        Image loadedImage = LoadImage(image);
        ImageFormat(&loadedImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

        Color trans = ColorFromTiledColor(transparentColor);
        ImageColorReplace(&loadedImage, trans, BLANK);

        // TODO: Support image tint color.

        Texture texture = LoadTextureFromImage(loadedImage);
        UnloadImage(loadedImage);
        data->textures[imagepath] = texture;
    }
}

Map LoadTiledFromMemory(const unsigned char *fileData, int dataSize, const char* baseDir) {
    tson::Tileson t;
    auto map = t.parse(fileData, dataSize);

    if (map == nullptr) {
        TraceLog(LOG_ERROR, "TILESON: Error parsing the given map");
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Check if it parsed okay.
    if(map->getStatus() != tson::ParseStatus::OK) {
        TraceLog(LOG_ERROR, "TILESON: Map parse error: %s", map->getStatusMessage().c_str());
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Only support Ortogonal
    if (map->getOrientation() != "orthogonal") {
        TraceLog(LOG_ERROR, "TILESON: Only support for orthogonal maps");
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Load all the images
    RaylibTilesonData* data = new RaylibTilesonData();
    for (const auto& layer : map->getLayers()) {
        if (layer.getType() == tson::LayerType::ImageLayer) {
            LoadTiledImage(data, baseDir, layer.getImage(), layer.getTransparentColor());
        }
    }
    for (const auto& tileset : map->getTilesets()) {
        LoadTiledImage(data, baseDir, tileset.getImage().string(), tileset.getTransparentColor());
    }

    // Save the Map.
    struct Map output;
    output.width = map->getSize().x;
    output.height = map->getSize().y;
    output.tileWidth = map->getTileSize().x;
    output.tileHeight = map->getTileSize().y;

    data->map = map.release();
    output.data = data;

    TraceLog(LOG_INFO, "TILESON: Map parsed successfully");
    return output;
}

Map LoadTiled(const char* fileName) {
    const char* baseDir = GetDirectoryPath(fileName);
    tson::Tileson t;
    auto map = t.parse(fileName);

    if (map == nullptr) {
        TraceLog(LOG_ERROR, "TILESON: Error parsing the given map");
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Check if it parsed okay.
    if(map->getStatus() != tson::ParseStatus::OK) {
        TraceLog(LOG_ERROR, "TILESON: Map parse error: %s", map->getStatusMessage().c_str());
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Only support Ortogonal
    if (map->getOrientation() != "orthogonal") {
        TraceLog(LOG_ERROR, "TILESON: Only support for orthogonal maps");
        struct Map output;
        output.data = NULL;
        return output;
    }

    // Load all the images
    RaylibTilesonData* data = new RaylibTilesonData();
    for (const auto& layer : map->getLayers()) {
        if (layer.getType() == tson::LayerType::ImageLayer) {
            LoadTiledImage(data, baseDir, layer.getImage(), layer.getTransparentColor());
        }
    }
    for (const auto& tileset : map->getTilesets()) {
        LoadTiledImage(data, baseDir, tileset.getImage().string(), tileset.getTransparentColor());
    }

    data->jsonFilePath = fileName;  

    // Save the Map.
    struct Map output;
    output.width = map->getSize().x;
    output.height = map->getSize().y;
    output.tileWidth = map->getTileSize().x;
    output.tileHeight = map->getTileSize().y;

    data->map = map.release();
    output.data = data;

    TraceLog(LOG_INFO, "TILESON: Map parsed successfully");
    return output;
}

bool IsTiledReady(Map map) {
    return map.data != NULL;
}

void DrawTileLayer(tson::Layer& layer, RaylibTilesonData* data, int posX, int posY, Color tint) {
    for (auto& [pos, tileObject] : layer.getTileObjects()) {
        const tson::Tileset *tileset = tileObject.getTile()->getTileset();
        std::string image = tileset->getImage().string();
        if (data->textures.count(image) == 0) {
            continue;
        }

        Texture texture = data->textures[image];
        Rectangle drawRect = RectangleFromTiledRectangle(tileObject.getDrawingRect());
        tson::Vector2f position = tileObject.getPosition();
        Vector2 drawPos = {position.x + posX, position.y + posY};
        DrawTextureRec(texture, drawRect, drawPos, tint);
    }
}


// Đọc flag repeatx/repeaty từ block JSON của layer có id = layerId
static void parseRepeatFlags(const std::string &js, int layerId, bool &outX, bool &outY) {
    std::string idKey = "\"id\":" + std::to_string(layerId);
    size_t p = js.find(idKey);
    if (p == std::string::npos) return;
    size_t end = js.find('}', p);
    // repeatx
    size_t rx = js.find("\"repeatx\":", p);
    if (rx != std::string::npos && rx < end && js.compare(rx+10,4,"true")==0) outX = true;
    // repeaty
    size_t ry = js.find("\"repeaty\":", p);
    if (ry != std::string::npos && ry < end && js.compare(ry+10,4,"true")==0) outY = true;
}

bool getRepeatX(const char* jsonFilePath, int layerId) {
    bool rx = false;
    char *txt = LoadFileText(jsonFilePath);
    if (txt) {
        std::string js(txt);
        UnloadFileText(txt);
        bool dummyY = false;
        parseRepeatFlags(js, layerId, rx, dummyY);
    }
    return rx;
}

bool getRepeatY(const char* jsonFilePath, int layerId) {
    bool ry = false;
    char *txt = LoadFileText(jsonFilePath);
    if (txt) {
        std::string js(txt);
        UnloadFileText(txt);
        bool dummyX = false;
        parseRepeatFlags(js, layerId, dummyX, ry);
    }
    return ry;
}

void DrawImageLayer(tson::Layer& layer, RaylibTilesonData* data, int posX, int posY, Color tint) {
    std::string image = layer.getImage();
    auto it = data->textures.find(image);
    if (it == data->textures.end()) return;
    Texture texture = it->second;

    // Lấy offset và kích thước map (pixel)
    auto offset = layer.getOffset();
    tson::Map* map = layer.getMap();
    int mapW = map->getSize().x * map->getTileSize().x;
    int mapH = map->getSize().y * map->getTileSize().y;

    int startX = posX + offset.x;
    int startY = posY + offset.y;
    
    bool repX = getRepeatX(data->jsonFilePath.c_str(), layer.getId());
    bool repY = getRepeatY(data->jsonFilePath.c_str(), layer.getId());

    if (repX || repY) {
        // Lặp vẽ ngang/dọc
        for (int x = startX; repX && x < mapW + startX; x += texture.width) {
            for (int y = startY; repY && y < mapH + startY; y += texture.height) {
                DrawTexture(texture, x, y, tint);
            }
            if (!repY) DrawTexture(texture, x, startY, tint);
        }
        if (!repX && repY) {
            for (int y = startY; y < mapH + startY; y += texture.height) {
                DrawTexture(texture, startX, y, tint);
            }
        }
    } else {
        // Vẽ 1 lần như cũ
        DrawTexture(texture, startX, startY, tint);
    }
}

static tson::Tile* getTile(tson::Map* map, uint32_t gid) {
    const auto &tileMap = map->getTileMap();               // API: ánh xạ GID → Tile* :contentReference[oaicite:0]{index=0}
    auto it = tileMap.find(gid);
    return (it != tileMap.end()) ? it->second : nullptr;
}

void DrawObjectLayer(tson::Layer &layer, RaylibTilesonData* data, int posX, int posY, Color tint) {
    // Lưu timer (ms) và frame index cho mỗi object-id
    static std::unordered_map<int, float>    timers;
    static std::unordered_map<int, size_t>   indices;

    tson::Map *tsonMap = layer.getMap();
    float dtMillis = GetFrameTime() * 1000.0f;

    for (auto &obj : layer.getObjects()) {
        if (!obj.isVisible() || obj.getGid() == 0) continue;

        uint32_t gid = obj.getGid();
        tson::Tile *tile = getTile(tsonMap, gid);
        if (!tile) continue;

        const tson::Tileset *ts = tsonMap->getTilesetByGid(gid);
        if (!ts) continue;

        // Lấy texture
        auto itTex = data->textures.find(ts->getImage().string());
        if (itTex == data->textures.end()) continue;
        Texture tex = itTex->second;

        // Lấy danh sách frames và durations từ animation
        auto &frames = tile->getAnimation().getFrames();
        if (frames.empty()) {
            // Không có animation: vẽ static
            Rectangle src = RectangleFromTiledRectangle(tile->getDrawingRect());
            Vector2 dst = { obj.getPosition().x + posX,
                            obj.getPosition().y + posY - src.height };
            DrawTextureRec(tex, src, dst, tint);
            continue;
        }

        // Cập nhật timer và chỉ số frame cho object này
        float &t = timers[obj.getId()];
        size_t &i = indices[obj.getId()];
        t += dtMillis;
        // Chuyển frame khi vượt duration
        while (t >= frames[i].getDuration()) {
            t -= frames[i].getDuration();
            i = (i + 1) % frames.size();
        }

        // Tự tính src Rect dựa trên tileid và số cột của tileset
        uint32_t frameTileId = frames[i].getTileId();
        int cols   = ts->getColumns();              // từ JSON `"columns"`
        int tileW  = ts->getTileSize().x;
        int tileH  = ts->getTileSize().y;
        Rectangle src = {
            float((frameTileId % cols) * tileW),
            float((frameTileId / cols) * tileH),
            float(tileW),
            float(tileH)
        };

        // Tính vị trí vẽ (Tiled quy y tại đáy)
        Vector2 dst = {
            obj.getPosition().x + posX,
            obj.getPosition().y + posY - tileH
        };

        DrawTextureRec(tex, src, dst, tint);
    }
}

bool getVisible(const RaylibTilesonData* data, tson::Layer &layer)
{
    // Try to open the JSON file
    std::ifstream inFile(data->jsonFilePath);
    if (!inFile.is_open())
    {
        TraceLog(LOG_WARNING,
                 "TILESON: Could not open JSON file '%s', defaulting visible = true",
                 data->jsonFilePath.c_str());
        return true;
    }

    // Parse
    nlohmann::json jsonData;
    try
    {
        inFile >> jsonData;
    }
    catch (const std::exception &e)
    {
        TraceLog(LOG_WARNING,
                 "TILESON: JSON parse error in '%s': %s, defaulting visible = true",
                 data->jsonFilePath.c_str(), e.what());
        return true;
    }

    // Look for layers array
    if (jsonData.contains("layers") && jsonData["layers"].is_array())
    {
        for (auto &layerJson : jsonData["layers"])
        {
            // Match by name (you can switch to "id" if you prefer)
            if (layerJson.value("name", "") == layer.getName())
            {
                // Return the explicit "visible" value, or true if it's missing
                return layerJson.value("visible", true);
            }
        }
    }

    // If we didn’t find a matching entry, default to visible
    return true;
}

void DrawLayer(tson::Layer &layer, RaylibTilesonData* data, int posX, int posY, Color tint) {
      if (!getVisible(data, layer))
        return;

    switch (layer.getType()) {
        case tson::LayerType::TileLayer:
            DrawTileLayer(layer, data, posX, posY, tint);
            break;

        case tson::LayerType::ObjectGroup:
            DrawObjectLayer(layer, data, posX, posY, tint);
            break;

        case tson::LayerType::ImageLayer:
            DrawImageLayer(layer, data, posX, posY, tint);
            break;

        case tson::LayerType::Group:
            for(auto &l : layer.getLayers()) {
                DrawLayer(l, data, posX, posY, tint);
            }
            break;

        default:
            TraceLog(LOG_TRACE, "TILESON: Unsupported layer type");
            break;
    }
}

void DrawTiled(Map map, int posX, int posY, Color tint) {
    RaylibTilesonData* data = (RaylibTilesonData*)map.data;
    if (data == NULL) {
        TraceLog(LOG_WARNING, "TILESON: Cannot draw empty map");
        return;
    }
    tson::Map* tsonMap = data->map;
    auto &layers = tsonMap->getLayers();

    // TODO: Draw the background color.

    for (auto &layer : layers) {
        DrawLayer(layer, data, posX, posY, tint);
    }
}

void UnloadMap(Map map) {
    RaylibTilesonData* data = (RaylibTilesonData*)map.data;
    if (data != NULL) {
        // Unload Tileson
        if (data->map != NULL) {
            delete data->map;
        }

        // Unload all the Textures
        for (const auto& [name, texture] : data->textures) {
            UnloadTexture(texture);
        }

        // Finally, delete the internal data
        delete data;
    };
}
