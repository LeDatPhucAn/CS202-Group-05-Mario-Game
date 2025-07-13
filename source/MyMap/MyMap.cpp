#include "MyMap.hpp"
#include <string>

void MyMap::choose(const std::string &jsonPath) {
	clearAll();
	// 1) Parse JSON via Tileson
	tson::Tileson parser;
	auto parsed = parser.parse(jsonPath);
	if (!parsed || parsed->getStatus() != tson::ParseStatus::OK) return;
	tsonMap = parsed.release();

	// 2) Cache each tileset texture + gather metadata (including margin/spacing)
	struct TSInfo { int firstgid, columns, margin, spacing; Vector2 tileSize; };
	std::vector<TSInfo> tsinfo;
	auto baseDir = std::filesystem::path(jsonPath).parent_path();
	for (auto &ts : tsonMap->getTilesets()) {
		std::string imgRel = ts.getImage().string();
		std::string path   = (baseDir / imgRel).string();
		Texture2D tex      = LoadTexture(path.c_str());

		int margin   = ts.getMargin();
		int spacing  = ts.getSpacing();
		int tileW    = ts.getTileSize().x;
		int tileH    = ts.getTileSize().y;
		int cols     = (tex.width  - 2*margin + spacing) / (tileW + spacing);

		tilesetCache[ts.getFirstgid()] = tex;
		tsinfo.push_back({ ts.getFirstgid(), cols, margin, spacing,
							{ float(tileW), float(tileH) } });
	}
	std::sort(tsinfo.begin(), tsinfo.end(),
				[](auto &a, auto &b){ return a.firstgid < b.firstgid; });

	// 3) Map dimensions
	int mapW    = tsonMap->getSize().x;
	int mapH    = tsonMap->getSize().y;
	int tileW   = tsonMap->getTileSize().x;
	int tileH   = tsonMap->getTileSize().y;

	mapBounds = Rectangle{ 0, 0, float(mapW * tileW), float(mapH * tileH) };

	// 4) Scan layers
	for (auto &layer : tsonMap->getLayers()) {
		switch (layer.getType()) {
			// =========IMAGE LAYER PROCESSING==============
			case tson::LayerType::ImageLayer: {
				cout << "IMAGE PROCESSING\n";
				auto imgRel = layer.getImage();
				std::string path = (baseDir / imgRel).string();
				Texture2D tex = LoadTexture(path.c_str());
				Rectangle src{ 0,0, float(tex.width), float(tex.height) };
				Vector2 pos{ layer.getOffset().x, layer.getOffset().y };
				imageBlocks.push_back(new Block(0, pos,
					{ float(tex.width), float(tex.height) }, tex, src));
				break;
			}

			// ==============TILE LAYER PROCESSING=============
			case tson::LayerType::TileLayer: {
				cout << "TILE PROCESSING\n";
				/*
				const auto &data = layer.getData();
				//cout << "DATA SIZE: " << (int)data.size() << "\n";
				for (int i = 0; i < (int)data.size(); ++i) {
					int gid = data[i]; if (gid == 0) continue; // wtf
					if (gid != 1) cout << "GID: " << gid << "\n";
					// select tileset info by gid
					const TSInfo* tsi = nullptr;
					for (int j = tsinfo.size()-1; j >= 0; --j)
						if (gid >= tsinfo[j].firstgid) { tsi = &tsinfo[j]; break; }
					if (!tsi) continue;

					int local = gid - tsi->firstgid;
					int col   = local % tsi->columns;
					int row   = local / tsi->columns;
					Rectangle src{
						tsi->margin + col * (tsi->tileSize.x + tsi->spacing),
						tsi->margin + row * (tsi->tileSize.y + tsi->spacing),
						tsi->tileSize.x,
						tsi->tileSize.y
					};
					int x = (i % mapW) * tileW;
					int y = (i / mapW) * tileH;
					// if (tsi->firstgid != 1) cout << "TSI->FIRSTGID: " << tsi->firstgid << "\n";
					Block* newBlock = nullptr;
					tileBlocks.push_back(new Block(
						gid,
						{ float(x), float(y) },
						{ float(tileW), float(tileH) },
						tilesetCache[tsi->firstgid], src
					));
				}
				break;
				*/

				for (auto &obj : layer.getTileData()) {
					std::cout << "CLASS: " << obj.second->getType() << "\n";
					int gid = obj.second->getGid(); if (gid == 0) continue;
					const TSInfo* tsi = nullptr;
					for (int j = tsinfo.size()-1; j >= 0; --j)
						if (gid >= tsinfo[j].firstgid) { tsi = &tsinfo[j]; break; }
					if (!tsi) continue;

					int local = gid - tsi->firstgid;
					int col   = local % tsi->columns;
					int row   = local / tsi->columns;
					Rectangle src{
						tsi->margin + col * (tsi->tileSize.x + tsi->spacing),
						tsi->margin + row * (tsi->tileSize.y + tsi->spacing),
						tsi->tileSize.x,
						tsi->tileSize.y
					};

					tileBlocks.push_back(new Block(
						1,
						{obj.second->getPosition(obj.first).x, obj.second->getPosition(obj.first).y},
						{obj.second->getTileSize().x, obj.second->getTileSize().y},
						tilesetCache[tsi->firstgid], src
					));
				}
			}

			// ============OBJECT LAYER PROCESSING================
			case tson::LayerType::ObjectGroup: {
				cout << "OBJECT PROCESSING\n";
				for (auto &obj : layer.getObjects()) {
					int gid = obj.getGid(); if (gid == 0) continue;
					cout << "OBJECT_GID: " << gid << "\n";
					const TSInfo* tsi = nullptr;
					for (int j = tsinfo.size()-1; j >= 0; --j)
						if (gid >= tsinfo[j].firstgid) { tsi = &tsinfo[j]; break; }
					if (!tsi) continue;
					int local = gid - tsi->firstgid;
					int col   = local % tsi->columns;
					int row   = local / tsi->columns;
					Rectangle src{
						tsi->margin + col * (tsi->tileSize.x + tsi->spacing),
						tsi->margin + row * (tsi->tileSize.y + tsi->spacing),
						tsi->tileSize.x,
						tsi->tileSize.y
					};
					Vector2 pos{ obj.getPosition().x,
									obj.getPosition().y - tsi->tileSize.y };
					objectBlocks.push_back(new Block(obj, pos,
						tsi->tileSize, tilesetCache[tsi->firstgid], src));
				}
				break;
			}
			default: break;
		}
	}
}

void MyMap::display(int ox, int oy) const {
	for (auto *b : imageBlocks)  drawBlock(b, ox, oy);
	for (auto *b : tileBlocks)   drawBlock(b, ox, oy);
	for (auto *b : objectBlocks) drawBlock(b, ox, oy);
}

void MyMap::update() {
	for (auto *b : objectBlocks) b->update();
}

Rectangle MyMap::getMapBounds() {
	return mapBounds;
}

void MyMap::clearAll() {
	for (auto *b : imageBlocks ) delete b;
	for (auto *b : tileBlocks  ) delete b;
	for (auto *b : objectBlocks) delete b;
	imageBlocks.clear(); 
	tileBlocks.clear(); 
	objectBlocks.clear();
	for (auto &kv : tilesetCache) UnloadTexture(kv.second);
	tilesetCache.clear();
}

void MyMap::drawBlock(const Block* b, int ox, int oy) {
	Vector2 p = b->getPosition();
	DrawTextureRec(b->texture, b->srcRec, { p.x + ox, p.y + oy }, WHITE);
}