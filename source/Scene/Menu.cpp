#include "Menu.hpp"
#include "raylib.h"
void Menu::displayScene() {
    DrawRectangle(100,100,600,600,RED);
    DrawText("Press N, then Press P in Game Scene", 200, 200, 40, BLACK);
    
}

void Menu::updateScene() { 
    if(IsKeyPressed(KEY_N))
        manager->changeScene(sceneType::GAME);
}