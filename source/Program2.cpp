#include "Program.h"
#include "raylib.h"

void Program::Init() {
    InitWindow(480,320, "GameMario");
    //MyMap map1;
}

void Program::Run() {
    Init();


    while(!WindowShouldClose()) {
        BeginDrawing(); 
            Update();
            Draw();
        EndDrawing();
    }
}

void Program::Update() {
    //mario.Update();
    //map1.Update();
}

void Program::Draw() {
    //map1.Update();
    //mario.Update();
}
