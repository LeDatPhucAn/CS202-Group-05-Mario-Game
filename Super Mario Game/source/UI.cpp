#include "../header/UI.hpp"
Font UI::font = { 0 };
int UI::screenWidth = 1600;
int UI::screenHeight = 900;
int UI::lastScreenWidth = 1600;
int UI::lastScreenHeight = 900;

vector<Texture2D> UI::Icons;
vector<Texture2D> UI::Buttons;
vector<Texture2D> UI::selectedButtons;
UI::UI() {
	font = LoadFont("assets/Fonts/JetBrainsMono-Regular.ttf");
	SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
	if (font.texture.id == 0) {
		cout << "didnt get da font";
		font = GetFontDefault();
	}

}

void UI::drawBackground() {
	Rectangle screen = { 0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight)};
	Rectangle source = { 1300, 300,static_cast<float>(screenWidth), static_cast<float>(screenHeight)};
	DrawTexturePro(UI::Icons[0], source, screen, { 0,0 }, 0, LIGHTGRAY);
}

void UI::drawLogo() {
	Rectangle logo = { 
		screenWidth / 2 - static_cast<float>(UI::Icons[1].width) / 2,
		static_cast<float> (screenHeight) / 64,
		static_cast<float> (UI::Icons[1].width), 
		static_cast<float> (UI::Icons[1].height) };
	DrawTexturePro(UI::Icons[1], { 0,0,(float)UI::Icons[1].width,(float)UI::Icons[1].height }, logo, { 0,0 }, 0, RAYWHITE);
}

void UI::initTextures() {

	//// Buttons
	//Buttons.push_back(LoadTexture("assets/Buttons/SinglyLinkedList.png"));
	//Buttons.push_back(LoadTexture("assets/Buttons/HashTable.png"));
	//Buttons.push_back(LoadTexture("assets/Buttons/Treap.png"));
	//Buttons.push_back(LoadTexture("assets/Buttons/Graph.png"));

	//// Selected Buttons
	//selectedButtons.push_back(LoadTexture("assets/Buttons/SinglyLinkedListSelected.png"));
	//selectedButtons.push_back(LoadTexture("assets/Buttons/HashTableSelected.png"));
	//selectedButtons.push_back(LoadTexture("assets/Buttons/TreapSelected.png"));
	//selectedButtons.push_back(LoadTexture("assets/Buttons/GraphSelected.png"));


	//// Icons
	Icons.push_back(LoadTexture("assets/Backgrounds/TitleScreen.jpg"));	// Icons[0]
	Icons.push_back(LoadTexture("assets/Backgrounds/logo.png"));		// Icons[1]
}


/// <summary>
/// Draw functions
/// </summary>

void UI::drawtext2(string message, int X, int Y, Color color) {
	const char* messageStr = message.c_str();

	// Measure text dimensions
	Vector2 textSize = MeasureTextEx(font, messageStr, fontSize, spacing);
	DrawText(messageStr, X - textSize.x / 2, Y - textSize.y / 2, fontSize, color);

}



/// <summary>
/// Cleanup functions
/// </summary>

void UI::UnLoadAllTextures() {
	for (const auto& texture : Icons) {
		UnloadTexture(texture);
	}
	Icons.clear();

	for (const auto& texture : Buttons) {
		UnloadTexture(texture);
	}
	Buttons.clear();
}

void UI::cleanup() {
	UnLoadAllTextures();
}

