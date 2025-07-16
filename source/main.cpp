
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
// #include "raylib.h"
// #include "box2d/box2d.h"
// using namespace std;
// #define PPM 50.0f // PIXEL PER METER
// int main()
// {
//     b2Vec2 gravity = {0, 9.8f};
//     b2World world(gravity);
//     b2BodyDef groundBodyDef;
//     groundBodyDef.fixedRotation = true;
//     groundBodyDef.position.Set(400.0f / PPM, 550.0f / PPM); // Convert to meters
//     b2Body *groundBody = world.CreateBody(&groundBodyDef);

//     b2PolygonShape groundBox;
//     groundBox.SetAsBox(200.0f / PPM, 10.0f / PPM);
//     groundBody->CreateFixture(&groundBox, 0); // Static = density 0

//     // 3. Create the dynamic falling box
//     b2BodyDef bodyDef;
//     bodyDef.type = b2_dynamicBody;
//     bodyDef.position.Set(200.0f / PPM, 100.0f / PPM); // Starting position
//     b2Body *dynamicBody = world.CreateBody(&bodyDef);

//     b2PolygonShape dynamicBox;
//     dynamicBox.SetAsBox(20.0f / PPM, 20.0f / PPM);

//     b2FixtureDef fixtureDef;
//     fixtureDef.shape = &dynamicBox;
//     fixtureDef.density = 1.0f;
//     fixtureDef.friction = 0.3f;
//     dynamicBody->CreateFixture(&fixtureDef);

//     InitWindow(800, 600, "Box2D Test");
//     SetTargetFPS(60);

//     while (!WindowShouldClose())
//     {
//         // Step the world
//         world.Step(1.0f / 60.0f, 6, 2);

//         BeginDrawing();
//         ClearBackground(RAYWHITE);

//         // Draw ground
//         DrawRectangleRec({200, 550, 400, 20}, DARKGRAY); // screen-space, in pixels

//         b2Vec2 pos = dynamicBody->GetPosition();
//         float angle = dynamicBody->GetAngle();

//         Vector2 boxPos = {(pos.x * PPM) - 20, (pos.y * PPM) - 20};
//         DrawRectanglePro(
//             {boxPos.x + 20, boxPos.y + 20, 40, 40}, // center origin
//             {20, 20},
//             angle * RAD2DEG,
//             RED);

//         DrawText("Press ESC to exit", 10, 10, 20, DARKGRAY);
//         EndDrawing();
//     }

//     CloseWindow();
// }