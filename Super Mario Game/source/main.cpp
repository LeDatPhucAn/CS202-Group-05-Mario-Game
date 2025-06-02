// Super Mario Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "../header/SceneHandler.hpp"
#include "../header/Button.hpp"
#include "../header/UI.hpp"
using namespace std;

int main()
{
	InitWindow(UI::screenWidth, UI::screenHeight, "Bruh");
	SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
	SetTargetFPS(60);
	UI ui;
	ui.initTextures();
	SceneHandler scenehandler;

	/// main functions
	while (!WindowShouldClose()) {

		// Get screen values
		UI::screenWidth = GetScreenWidth();
		UI::screenHeight = GetScreenHeight();

		// Update
		//----------------------------------------------------------------------------------
		scenehandler.updateCurrentScene();

		//----------------------------------------------------------------------------------


		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		ClearBackground(RAYWHITE);

		scenehandler.displayCurrentScene();
		
		
		EndDrawing();
		//----------------------------------------------------------------------------------

	}
	CloseWindow();
	ui.cleanup();
}