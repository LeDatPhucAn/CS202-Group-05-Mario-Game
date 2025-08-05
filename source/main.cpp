
#include <iostream>
#include <string>
#include "Program.hpp"
#include "UI.hpp"
using namespace std;
int main()
{
    srand(time(NULL));
    InitWindow(UI::screenWidth, UI::screenHeight, "Game Mario");
    InitAudioDevice();
    SetTargetFPS(60);

    UI ui;

    Program program;
    program.run();
    CloseAudioDevice();
}
