
#include <iostream>
#include <string>
#include "Program.hpp"
#include "UI.hpp"
using namespace std;
int main()
{
    InitWindow(UI::screenWidth, UI::screenHeight, "Game Mario");
    SetTargetFPS(60);
    
    UI ui;
    
    Program program;
    program.run();
}   
