#include "Program.hpp"
#include "raylib.h"
#include<iostream>

using namespace std;
Program::Program() {
    InitWindow(screenWidth,screenHeight, "Game Mario");
    SetTargetFPS(60);
    mySceneManager.init();
    mySceneManager.pro = this;
    

}

void Program::update() {
    mySceneManager.update();
}

void Program::display() {
    mySceneManager.display();
}


void Program::run() {
    /// main functions
    while (!WindowShouldClose()) {

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