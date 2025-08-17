#include "WinLevel.hpp"
#include "Menu.hpp"
#include "Scene.hpp"
#include "Game.hpp"
#include <raylib.h>

WinLevel::WinLevel(SceneManager* manager)
    : Scene(manager), anyKeyPressed(false) {
        mapName = manager->curMap; 
    }

WinLevel::~WinLevel() {}

void WinLevel::updateScene() {

    if (!anyKeyPressed && GetKeyPressed()!=0) {
        anyKeyPressed = true;
        if (mapName == "Map1" || mapName == "Map2") {
            // Move to next level
            if (manager->curMap == "Map1") manager->curMap = "Map2";
            else if (manager->curMap == "Map2") manager->curMap = "Map3";
            manager->shouldReloadGame = true;
            manager->goBack();
        } else if (mapName == "Map3") {
            manager->goBackOfBaseScene(); // This should go to main menu
        }
    }
}

void WinLevel::displayScene() {
    ClearBackground(BLACK);
    
    
    const char* title = "";
    const char* subtitle = "";
    const char* prompt = "";
    static string titleStr = "";

    if (mapName == "Map1" || mapName == "Map2") {
        titleStr = (mapName == "Map1") ? "Level 1 Completed" : "Level 2 Completed";
        title = titleStr.c_str();
        subtitle = "You almost saved the princess, keep going";
        prompt = "Press any key to move to next level";
    } else if (mapName == "Map3") {
        title = "Victory";
        subtitle = "You have saved the princess";
        prompt = "Press any key to back to the menu";
    }
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int fontSize = 60;
    int subFontSize = 30;
    int promptFontSize = 24;
    
    DrawText(title, screenWidth/2 - MeasureText(title, fontSize)/2, screenHeight/2 - 80, fontSize, YELLOW);
    if (subtitle && subtitle[0])
        DrawText(subtitle, screenWidth/2 - MeasureText(subtitle, subFontSize)/2, screenHeight/2, subFontSize, WHITE);
    DrawText(prompt, screenWidth/2 - MeasureText(prompt, promptFontSize)/2, screenHeight/2 + 60, promptFontSize, GRAY);
}