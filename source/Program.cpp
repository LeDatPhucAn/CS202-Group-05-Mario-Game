#include "Program.hpp"
#include "raylib.h"
#include <iostream>
#include "Structs.hpp"
#include "UI.hpp"
#include "KeyBindingManager.hpp"
using namespace std;
Program::Program()
{   

    mySceneManager.init();
    mySceneManager.pro = this;
}


void Program::update()
{
    mySceneManager.update();
}

void Program::display()
{
    mySceneManager.display();
}

void Program::run()
{
    /// main functions
    KeyBindingManager::loadFromFile();
    while (!WindowShouldClose())
    {
        update();
        BeginDrawing();
            BeginMode2D(mySceneManager.getCamera());

            ClearBackground(WHITE);
            display();

            EndMode2D();
        EndDrawing();
    }
    CloseWindow();
}