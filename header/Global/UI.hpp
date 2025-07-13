#pragma once
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
#include <fstream>
#include "json.hpp"
#include "tileson.hpp"
#include "raylib-tileson.h"

using json = nlohmann::json;

using namespace std;


class UI {
public:

	UI();
	~UI();

	static Font font;
	static constexpr float spacing = 2.0f;
	static constexpr float fontSize = 30.0f;

	static int screenWidth;
	static int screenHeight;
	static int lastScreenWidth;
	static int lastScreenHeight;

	static unordered_map<string, json> jsonMap;
	static unordered_map<string, Texture2D> textureMap;
	static unordered_map<string, Map> gameMap;
	static vector<Rectangle> JsonToRectangleVector(const json& Json);
	static void drawBackground();
	static void drawLogo();
	static void drawtext2(string message, int X, int Y, Color color);

	static Vector2 getMaxTextSize(int charCount) {
		std::string sampleText(charCount, 'W'); // Use 'W' as widest character
		return MeasureTextEx(UI::font, sampleText.c_str(), UI::fontSize, UI::spacing);
	}
	static Color interpolateColors(Color& color1, Color& color2, float t) {
		Color color;
		color.r = (unsigned char)(color1.r + (color2.r - color1.r) * t);
		color.g = (unsigned char)(color1.g + (color2.g - color1.g) * t);
		color.b = (unsigned char)(color1.b + (color2.b - color1.b) * t);
		color.a = (unsigned char)(color1.a + (color2.a - color1.a) * t);
		return color;
	}
	static void darkenColor(Color& color, int amount) {
		color.r = (unsigned char)(color.r - amount < 0 ? 0 : color.r - amount);
		color.g = (unsigned char)(color.g - amount < 0 ? 0 : color.g - amount);
		color.b = (unsigned char)(color.b - amount < 0 ? 0 : color.b - amount);
	}
	static void lightenColor(Color& color, int amount) {
		color.r = (unsigned char)(color.r + amount > 255 ? 255 : color.r + amount);
		color.g = (unsigned char)(color.g + amount > 255 ? 255 : color.g + amount);
		color.b = (unsigned char)(color.b + amount > 255 ? 255 : color.b + amount);
	}


private:
	void initBackgrounds();
	void initSprites();
	void initGameMaps();
	void UnLoadAllResources();

};