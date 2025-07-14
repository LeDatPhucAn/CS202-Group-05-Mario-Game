
#include <iostream>
#include <string>
#include "Program.hpp"
using namespace std;

int main()
{
    // Initialize window
    InitWindow(UI::screenWidth, UI::screenHeight, "Bruh");
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);

    // run program
    UI ui;
    Program program;
    program.run();
}