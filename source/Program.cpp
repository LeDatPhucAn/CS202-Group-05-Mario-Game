#include "Program.hpp"
#include "Menu.hpp"
#include "Game.hpp"
#include "UI.hpp"
#include "Character.hpp"
Vector2 Program::mouseWorldPos = { 0, 0 };

Program::Program()
{
    
    camera.zoom = 1.0f;
    UI::screenWidth = GetScreenWidth();
    UI::screenHeight = GetScreenHeight();

    // intialize scenes
    scenes.push_back(new Menu(this));
    scenes.push_back(new Game());
    
    // initialize Scene Buttons
    changeScene(GAME);
    initButtons();
}

Program::~Program() {
    for (int i = 0; i < scenes.size(); i++) {
        delete scenes[i];
    }
    Button::deleteButtons<Button>(SceneButtons);
}

void Program::initButtons() {

    Button* MenuButton = new TextBox("Menu", UI::screenWidth / 100, UI::screenHeight / 100);

    MenuButton->onClick = [this]() {
        this->changeScene(MENU);
    };


    SceneButtons.push_back(MenuButton);
    
}

int Program::getCurrentScene() {
    return currentSceneObject->CurrentScene;
}

void Program::changeScene(sceneType newScene) {
    Button::resetButtonsAnimations<Button>(SceneButtons);

    currentSceneObject = scenes[newScene];
    currentSceneObject->CurrentScene = newScene;
}

void Program::updateCamera() {
    // Move camera with drag
    if (!Button::isClicking && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f / camera.zoom);
        camera.target = Vector2Add(camera.target, delta);
    }

    // Handle zoom with scroll wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        Program::mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera); // update position
        camera.offset = GetMousePosition();
        camera.target = Program::mouseWorldPos;

        float scaleFactor = 1.0f + (0.25f * fabsf(wheel));
        if (wheel < 0) scaleFactor = 1.0f / scaleFactor;
        camera.zoom = Clamp(camera.zoom * scaleFactor, 0.25f, 10.0f);
    }

    // Always keep it up-to-date, even without zoom
    Program::mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
}



void Program::updateCurrentScene() {
    if (currentSceneObject) {

        // update font size
        // update The Positions of all Scenes when there is a Window Resize
        if (UI::lastScreenWidth != UI::screenWidth || UI::lastScreenHeight != UI::screenHeight) {
            

            // Menu Button
            dynamic_cast<RectButton*>(SceneButtons[0])->setPosition(UI::screenWidth / 100, UI::screenHeight / 100);         

            Button::resetButtonsAnimations<Button>(SceneButtons);


            UI::lastScreenWidth = UI::screenWidth;
            UI::lastScreenHeight = UI::screenHeight;
        }

        Button::isCollision = false;
        Button::updateButtons<Button>(SceneButtons);
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            Button::isClicking = false;
        }



        if (getCurrentScene() != MENU) {
            updateCamera();
            currentSceneObject->updateSceneInCamera(camera);

        }
        
        currentSceneObject->updateScene();

    }
}

void Program::displayCurrentScene() {
    if (currentSceneObject) {

        //apply camera to data structures
        if (getCurrentScene() == GAME) {

            BeginMode2D(camera);
            // Draw the 3d grid, rotated 90 degrees and centered around 0,0 
            // just so we have something in the XY plane
            rlPushMatrix();
            rlTranslatef(0, 10000, 0);
            rlRotatef(90, 1, 0, 0);
            DrawGrid(1000, 100);
            rlPopMatrix();
            currentSceneObject->displaySceneInCamera();
            EndMode2D();

            // Draw the buttons that will be on every scene
            Button::drawButtons<Button>(SceneButtons);
        }     
        // display permanent objects
        currentSceneObject->displayScene();
    }
    

    // Draw mouse reference
    DrawCircleV(GetMousePosition(), 4, DARKGRAY);
    DrawTextEx(GetFontDefault(), TextFormat("[%i, %i]", GetMouseX(), GetMouseY()),
    Vector2Add(GetMousePosition(), { -44, -24 }), 20, 2, BLACK);
}


void Program::run() {
    /// main functions
    while (!WindowShouldClose()) {
        // Get screen values
        UI::screenWidth = GetScreenWidth();
        UI::screenHeight = GetScreenHeight();

        // Update
       updateCurrentScene();
        // Draw
         BeginDrawing();
            ClearBackground(RAYWHITE);
            displayCurrentScene();
        EndDrawing();
    }
    CloseWindow();
}