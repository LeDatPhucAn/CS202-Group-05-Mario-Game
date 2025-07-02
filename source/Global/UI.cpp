#include "UI.hpp"
#include "ProcessJson.hpp"
#include "raylib-tileson.h"
#include <filesystem>
namespace fs = std::filesystem;
// initialize static members
Font UI::font = { 0 };
int UI::screenWidth = 1600;
int UI::screenHeight = 900;
int UI::lastScreenWidth = 1600;
int UI::lastScreenHeight = 900;
unordered_map<string, Texture2D> UI::textureMap;
unordered_map<string, json> UI::jsonMap;
unordered_map<string, Map> UI::gameMap; 
UI::UI() {
	cout<<"UI CONSTRUCTOR\n";
	initGameMaps();
	initSprites();
	initBackgrounds();
	font = LoadFont("assets/Fonts/JetBrainsMono-Regular.ttf");
	
	SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
	
	if (font.texture.id == 0) {
		cout << "didnt get da font";
		font = GetFontDefault();
	}

}

UI::~UI() {
	UnLoadAllResources();
}

vector<Rectangle> UI::JsonToRectangleVector(const json& Json) {
	vector<Rectangle> rectangle;

	Json.size();
	for (const auto& Rect : Json) {
		rectangle.push_back({ Rect["x"], Rect["y"], Rect["width"], Rect["height"] });
	}
	return rectangle;
}
void UI::drawBackground() {
	Rectangle screen = { 0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight)};
	Rectangle source = { 1300, 300,static_cast<float>(screenWidth), static_cast<float>(screenHeight)};

	DrawTexturePro(UI::textureMap["TitleScreen"], source, screen, {0,0}, 0, LIGHTGRAY);
}

void UI::drawLogo() {
	Texture2D t = UI::textureMap["Logo"];
	Rectangle logo = { 
		screenWidth / 2 - static_cast<float>(t.width) / 2,
		static_cast<float> (screenHeight) / 64,
		static_cast<float> (t.width), 
		static_cast<float> (t.height) };
	DrawTexturePro(t, { 0,0,(float)t.width,(float)t.height }, logo, { 0,0 }, 0, RAYWHITE);
}


void UI::initBackgrounds() {
	const string backgroundPath = "assets/Backgrounds/";

	for (const auto& entry : fs::directory_iterator(backgroundPath)) {
		textureMap[entry.path().stem().string()] = LoadTexture(entry.path().string().c_str());
	}
}

void UI::initSprites() {
	const std::string spritePath = "assets/Sprites/";

	for (const auto& entry : fs::directory_iterator(spritePath)) {
		if (entry.path().extension() == ".json") {
			std::string name = entry.path().stem().string(); // e.g., "Mario2D"
			fs::path pngFile = spritePath + name + ".png";

			if (fs::exists(pngFile)) {
				jsonMap[name] = getProcessedSpriteJson(entry.path().string());
				textureMap[name] = LoadTexture(pngFile.string().c_str());
			}
		}
	}
}

void UI::initGameMaps() {
	// const std::string mapPath = "assets/Map/";

	// for (const auto& entry : fs::directory_iterator(mapPath)) {
	// 	if (entry.path().extension() == ".json") {
	// 		gameMap[entry.path().stem().string()] = LoadTiled(entry.path().string().c_str());
	// 	}
	// }
}

void UI::drawtext2(string message, int X, int Y, Color color) {
	const char* messageStr = message.c_str();

	// Measure text dimensions
	Vector2 textSize = MeasureTextEx(font, messageStr, fontSize, spacing);
	DrawText(messageStr, X - textSize.x / 2, Y - textSize.y / 2, fontSize, color);

}

void UI::UnLoadAllResources() {
	for (const auto& texture : textureMap) {
		UnloadTexture(texture.second);
	}
	for(const auto &x : gameMap){
		UnloadMap(x.second);
	}
}



