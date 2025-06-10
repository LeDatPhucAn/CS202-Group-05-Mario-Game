// Super Mario Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "../header/SceneHandler.hpp"
#include "../header/Button.hpp"
#include "../header/UI.hpp"
#include "../source/Map/Map.hpp"
using namespace std;

int main()
{
	InitWindow(UI::screenWidth, UI::screenHeight, "Bruh");
	SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
	SetTargetFPS(60);
	UI ui;
	SceneHandler scenehandler;

	Map gameMap;
    if (!gameMap.loadFromFile("assets/Json/stage-1-1.json")) {
        std::cerr << "Failed to load map\n";
        return 1;
    }

	while (!WindowShouldClose()) {
		UI::screenWidth = GetScreenWidth();
		UI::screenHeight = GetScreenHeight();

		scenehandler.updateCurrentScene();

		BeginDrawing();
		ClearBackground(RAYWHITE);

		gameMap.draw();
		scenehandler.displayCurrentScene();
		
		
		EndDrawing();
	}
	CloseWindow();
}