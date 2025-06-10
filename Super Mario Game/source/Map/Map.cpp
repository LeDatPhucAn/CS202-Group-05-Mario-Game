#include "Map.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

// =====================================
// Tileset implementation
// =====================================

Tileset::~Tileset() {
    unload();
}

void Tileset::unload() {
    if (_texture.id != 0) {
        UnloadTexture(_texture);
        _texture.id = 0;
    }
}

Tileset::Tileset(Tileset&& other) noexcept
    : _firstGid(other._firstGid),
      _name(std::move(other._name)),
      _texture(other._texture),
      _tileWidth(other._tileWidth),
      _tileHeight(other._tileHeight),
      _columns(other._columns),
      _tileCount(other._tileCount),
      _tileRects(std::move(other._tileRects))
{
    other._texture.id = 0;
}

Tileset& Tileset::operator=(Tileset&& other) noexcept {
    if (this != &other) {
        unload();
        _firstGid   = other._firstGid;
        _name       = std::move(other._name);
        _texture    = other._texture;
        _tileWidth  = other._tileWidth;
        _tileHeight = other._tileHeight;
        _columns    = other._columns;
        _tileCount  = other._tileCount;
        _tileRects  = std::move(other._tileRects);
        other._texture.id = 0;
    }
    return *this;
}

void Tileset::loadFromJson(const json& tsJson) {
    _firstGid   = tsJson.at("firstgid").get<int>();
    _name       = tsJson.value("name", std::string(""));
    _tileWidth  = tsJson.at("tilewidth").get<int>();
    _tileHeight = tsJson.at("tileheight").get<int>();

    int margin  = tsJson.value("margin", 0);
    int spacing = tsJson.value("spacing", 0);
    _columns   = 15;
    _tileCount = 229;

    std::string imgPath = tsJson.at("image").get<std::string>();
    imgPath = "assets/Json/" + imgPath;
    _texture = LoadTexture(imgPath.c_str());
    if (_texture.id == 0) {
        throw std::runtime_error("Failed to load tileset texture: " + imgPath);
    }

    _tileRects.clear();
    _tileRects.reserve(_tileCount);
    for (int i = 0; i < _tileCount; ++i) {
        int col = i % _columns;
        int row = i / _columns;
        Rectangle r;
        r.x      = margin + col * (_tileWidth + spacing);
        r.y      = margin + row * (_tileHeight + spacing);
        r.width  = _tileWidth;
        r.height = _tileHeight;
        _tileRects.push_back(r);
    }
}

Rectangle Tileset::getSrcRect(int gid) const {
    int local = gid - _firstGid;
    if (local < 0 || local >= _tileCount) {
        throw std::out_of_range("GID out of range in Tileset::getSrcRect");
    }
    return _tileRects[local];
}

// =====================================
// Layer implementation
// =====================================

void Layer::loadFromJson(const json& layerJson) {
    _name    = layerJson.value("name", std::string(""));
    _width   = layerJson.at("width").get<int>();
    _height  = layerJson.at("height").get<int>();
    _visible = layerJson.value("visible", true);
    _opacity = layerJson.value("opacity", 1.0f);

    auto& arr = layerJson.at("data");
    if (arr.size() != static_cast<size_t>(_width * _height)) {
        throw std::runtime_error("Layer data size mismatch");
    }
    _data.clear();
    _data.reserve(arr.size());
    for (auto& v : arr) {
        _data.push_back(v.get<int>());
    }
}

int Layer::getTileGid(int x, int y) const {
    return _data.at(y * _width + x);
}

void Layer::setTileGid(int x, int y, int gid) {
    _data.at(y * _width + x) = gid;
}

void Layer::draw(const std::vector<Tileset>& tilesets,
                 int globalTileWidth, int globalTileHeight) const
{
    if (!_visible) return;

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            int gid = getTileGid(x, y);
            if (gid == 0) continue;

            const Tileset* ts = nullptr;
            for (const auto& t : tilesets) {
                if (gid >= t.getFirstGid() && gid < t.getFirstGid() + t.getTileCount()) {
                    ts = &t;
                    break;
                }
            }
            if (!ts) continue;

            Rectangle src = ts->getSrcRect(gid);
            Vector2 pos{ float(x * globalTileWidth), float(y * globalTileHeight) };
            DrawTextureRec(ts->getTexture(), src, pos,
                           Color{255,255,255, static_cast<unsigned char>(_opacity * 255)});
        }
    }
}

// =====================================
// Map implementation
// =====================================

const Tileset* Map::findTilesetForGid(int gid) const {
    for (const auto& ts : _tilesets) {
        if (gid >= ts.getFirstGid() && gid < ts.getFirstGid() + ts.getTileCount()) {
            return &ts;
        }
    }
    return nullptr;
}

bool Map::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open map file: " << filename << "\n";
        return false;
    }

    json j;
    try {
        j = json::parse(file);
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error in map file: " << e.what() << "\n";
        return false;
    }

    _width      = j.at("width").get<int>();
    _height     = j.at("height").get<int>();
    _tileWidth  = j.at("tilewidth").get<int>();
    _tileHeight = j.at("tileheight").get<int>();

    std::string mapDir;
    auto pos = filename.find_last_of("/\\");
    if (pos != std::string::npos) {
        mapDir = filename.substr(0, pos + 1);
    }

    _tilesets.clear();
    for (auto& tsj : j.at("tilesets")) {
        Tileset ts;
        std::string source = tsj.at("source").get<std::string>();
        for (auto& c : source) if (c == '\\') c = '/';
        std::string path = mapDir + source;

        std::ifstream tsfile(path);
        if (!tsfile) {
            std::cerr << "Cannot open tileset file: " << path << "\n";
            return false;
        }

        json tsjson;
        try {
            tsjson = json::parse(tsfile);
        } catch (const json::parse_error& e) {
            std::cerr << "JSON parse error in tileset: " << e.what() << "\n";
            return false;
        }

        tsjson["firstgid"] = tsj.at("firstgid").get<int>();
        ts.loadFromJson(tsjson);
        _tilesets.push_back(std::move(ts));
    }

    _layers.clear();
    for (auto& lj : j.at("layers")) {
        if (lj.at("type").get<std::string>() == "tilelayer") {
            Layer layer;
            layer.loadFromJson(lj);
            _layers.push_back(std::move(layer));
        }
    }

    return true;
}

void Map::draw() const {
    for (const auto& layer : _layers) {
        layer.draw(_tilesets, _tileWidth, _tileHeight);
    }
}

Layer* Map::getLayerByName(const std::string& name) {
    for (auto& layer : _layers) {
        if (layer._name == name) return &layer;
    }
    return nullptr;
}
