#include "Program.hpp"
#include "raylib.h"
#include <iostream>
#include "UI.hpp"
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
    while (!WindowShouldClose())
    {

        // Update
        update();
        // Draw
        BeginDrawing();
        BeginMode2D(mySceneManager.getCamera());

        ClearBackground(WHITE);
        display();

        EndMode2D();
        EndDrawing();
    }
    CloseWindow();
}