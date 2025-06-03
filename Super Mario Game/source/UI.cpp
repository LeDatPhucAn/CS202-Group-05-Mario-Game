#include "../header/UI.hpp"


// initialize static members
Font UI::font = { 0 };
int UI::screenWidth = 1600;
int UI::screenHeight = 900;
int UI::lastScreenWidth = 1600;
int UI::lastScreenHeight = 900;
unordered_map<string, Texture2D> UI::textureMap;
unordered_map<string, json> UI::jsonMap;

UI::UI() {
	initJson();
	initTextures();
	
	font = LoadFont("assets/Fonts/JetBrainsMono-Regular.ttf");
	
	SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
	
	if (font.texture.id == 0) {
		cout << "didnt get da font";
		font = GetFontDefault();
	}

}

UI::~UI() {
	UnLoadAllTextures();
}

vector<Rectangle> UI::JsonToRectangleVector(const json& Json) {
	vector<Rectangle> rectangle;
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

void UI::initTextures() {

	std::unordered_map<std::string, std::string> texturePaths = {
		{"TitleScreen", "assets/Backgrounds/TitleScreen.jpg"},
		{"Logo", "assets/Backgrounds/logo.png"},
		{"Mario", "assets/Sprites/mario.png"}
		// Add the rest...
	};

	for (const auto& KeyAndPath : texturePaths) {
		textureMap[KeyAndPath.first] = LoadTexture(KeyAndPath.second.c_str());
	}
}
void UI::initJson() {

	std::unordered_map<std::string, std::string> jsonPaths = {
		{"Mario", "assets/Json/mario.json"}
		// Add the rest...
	};

	ifstream file;
	for (const auto& KeyAndPath : jsonPaths) {
		file.open(KeyAndPath.second);
		file >> jsonMap[KeyAndPath.first];
		for (const auto& sprite : jsonMap[KeyAndPath.first]) {
			std::string name = sprite["name"];
			int x = sprite["x"];
			int y = sprite["y"];
			int width = sprite["width"];
			int height = sprite["height"];

			std::cout << name << ": (" << x << ", " << y << "), "
				<< width << "x" << height << "\n";
		}
		file.close();
	}
}


void UI::drawtext2(string message, int X, int Y, Color color) {
	const char* messageStr = message.c_str();

	// Measure text dimensions
	Vector2 textSize = MeasureTextEx(font, messageStr, fontSize, spacing);
	DrawText(messageStr, X - textSize.x / 2, Y - textSize.y / 2, fontSize, color);

}

void UI::UnLoadAllTextures() {
	for (const auto& texture : textureMap) {
		UnloadTexture(texture.second);
	}
}



