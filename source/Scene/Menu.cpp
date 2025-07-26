#include "Menu.hpp"
#include "raylib.h"
#include <cmath>
#include <vector>
#include <string>

// Static variable for title animation
static float titleBounce = 0.0f;
static bool buttonsInitialized = false;

Menu::Menu(SceneManager* _manager) : Scene(_manager) {
    // Load the background texture
    backgroundTexture = LoadTexture("assets/Backgrounds/MenuBackground.png");
    
    if (backgroundTexture.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load menu background texture");
    }

    // Load button textures
    buttonTexture = LoadTexture("assets/Backgrounds/Buttons/MenuButton.png");  // Your orange button
    buttonHoverTexture = LoadTexture("assets/Backgrounds/Buttons/MenuButtonHovered.png"); // Your darker orange button

    if (buttonTexture.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load button texture");
    }
    if (buttonHoverTexture.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load button hover texture");
    }

    //Load button textures into vector
    vector<std::string> iconPaths = {
        "assets/Backgrounds/Buttons/StartButtonIcon.png",
        "assets/Backgrounds/Buttons/ChooseLVButtonIcon.png",
        "assets/Backgrounds/Buttons/SettingButtonIcon.png",
        "assets/Backgrounds/Buttons/ExitButtonIcon.png"
    };

    for (const auto& path : iconPaths) {
        Texture2D texture = LoadTexture(path.c_str());
        if (texture.id == 0) {
            TraceLog(LOG_WARNING, "Failed to load button icon texture");
        }
        buttonTextures.push_back(texture);
    }
}

Menu::~Menu() {
    // Clean up texture
    if (backgroundTexture.id > 0) {
        UnloadTexture(backgroundTexture);
    }

     if (buttonTexture.id > 0) {
        UnloadTexture(buttonTexture);
    }
    if (buttonHoverTexture.id > 0) {
        UnloadTexture(buttonHoverTexture);
    }
    
    // Clean up buttons
    for (auto button : buttons) {
        delete button;
    }
    buttons.clear();

    // Clean up button textures
    for (auto& texture : buttonTextures) {
        if (texture.id > 0) {
            UnloadTexture(texture);
        }
    }
    buttonTextures.clear();
}

void Menu::updateScene() {

    
    // Initialize buttons once
    if (!buttonsInitialized) {
        std::vector<std::string> buttonTexts = {
            "START GAME",
            "CHOOSE LEVEL", 
            "SETTINGS",
            "EXIT GAME"
        };
        
       
        
        for (int i = 0; i < buttonTexts.size(); i++) {
            buttons.push_back(new TextBox(buttonTexts[i], 0, 0, WHITE, ORANGE, BLACK));
        }
        
        // Set button click handlers
        buttons[0]->onClick = [this]() { manager->changeScene(sceneType::GAME); };
        buttons[1]->onClick = [this]() { manager->changeScene(sceneType::CHOOSE_LEVEL); };
        buttons[2]->onClick = [this]() { manager->changeScene(sceneType::SETTING); };
        buttons[3]->onClick = [this]() { CloseWindow(); };
        
        buttonsInitialized = true;
    }
    
    titleBounce += GetFrameTime() * 2.0f;
    
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        selectedButton = (selectedButton - 1 + 4) % 4;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        selectedButton = (selectedButton + 1) % 4;
    }
    
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        switch(selectedButton) {
            case 0: manager->changeScene(sceneType::GAME); break;
            case 1: manager->changeScene(sceneType::CHOOSE_LEVEL); break;
            case 2: manager->changeScene(sceneType::SETTING); break;
            case 3: CloseWindow(); break;
        }
    }

    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int buttonWidth = 250;
        int buttonHeight = 60;
        int buttonX = UI::screenWidth / 2 - buttonWidth / 2;
        int startY = UI::screenHeight / 2 - 100;
        int spacing = 80;
        
        for (int i = 0; i < 4; i++) {
            Rectangle buttonRect = {buttonX, startY + i * spacing, buttonWidth, buttonHeight};
            if (CheckCollisionPointRec(mousePos, buttonRect)) {
                selectedButton = i;
                // Execute button action
                switch(i) {
                    case 0: manager->changeScene(sceneType::GAME); break;
                    case 1: manager->changeScene(sceneType::CHOOSE_LEVEL); break;
                    case 2: manager->changeScene(sceneType::SETTING); break;
                    case 3: CloseWindow(); break;
                }
                break;
            }
        }
    }
    
    if (IsKeyPressed(KEY_N)) {
        manager->changeScene(sceneType::GAME);
    }
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
    }
    
    // Update buttons for mouse interaction
    for(auto button : buttons) {
        button->update();
    }
    

}

void Menu::displayScene() {
    //Texts
    std::string titleText = "SUPER MARIO BROS";
    std::string subtitleText = "CS202 Group 05";
    std::string instructionsText = "Use ARROW KEYS/W-S to navigate \n ENTER/SPACE to select \n ESC to exit \n N for quick start";

    std::vector<std::string> buttonTexts = {
        "START GAME",
        "CHOOSE LEVEL", 
        "SETTINGS",
        "EXIT GAME"
    };
    
    //Load background textures
    if (backgroundTexture.id > 0) {
        DrawTexturePro(backgroundTexture, 
                      {0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
                      {0, 0, (float)UI::screenWidth, (float)UI::screenHeight},
                      {0, 0}, 0.0f, WHITE);
    } else {
        ClearBackground(BLACK);
    }
    
    // Bouncing title effect
    float bounceOffset = sin(titleBounce) * 20.0f;
    
    // Title centering
    float titleSpacing = 12.0f;
    float titleFontSize = 72.0f;
    
    float titleX = (UI::screenWidth - 750) / 2.0f;
    float titleY = 80 + bounceOffset;
    
    //Subtitle centering
    float subtitleSpacing = 5.0f; 
    float subtitleFontSize = 40.0f;
    
    float subtitleX = (UI::screenWidth - 350) / 2.0f; 
    float subtitleY = 170 + bounceOffset;
    
    // Draw title with outline effect
    int outlineThickness = 4;
    for (int dx = -outlineThickness; dx <= outlineThickness; dx++) {
        for (int dy = -outlineThickness; dy <= outlineThickness; dy++) {
            if (dx != 0 || dy != 0) {
                DrawTextEx(UI::boldFont, titleText.c_str(), 
                          {titleX + dx, titleY + dy}, titleFontSize, titleSpacing, ORANGE);
            }
        }
    }
    DrawTextEx(UI::boldFont, titleText.c_str(), 
              {titleX, titleY}, titleFontSize, titleSpacing, RED);
    
    // Draw subtitle with shadow
    DrawTextEx(UI::boldFont, subtitleText.c_str(), 
              {subtitleX + 2, subtitleY + 2}, subtitleFontSize, subtitleSpacing, DARKBROWN);
    DrawTextEx(UI::boldFont, subtitleText.c_str(), 
              {subtitleX, subtitleY}, subtitleFontSize, subtitleSpacing, ORANGE);
    
    // Position and draw custom textured buttons
    float buttonWidth = 250;
    float buttonHeight = 60;
    float buttonX = UI::screenWidth / 2 - buttonWidth / 2;
    float startY = UI::screenHeight / 2 - 100;
    float spacing = 80;
    
    for (int i = 0; i < 4; i++) {
        Rectangle buttonRect = {buttonX, startY + i * spacing, buttonWidth, buttonHeight};
        
        // Choose texture based on selection
        Texture2D currentTexture = (i == selectedButton) ? buttonHoverTexture : buttonTexture;
        
        // Draw button texture
        if (currentTexture.id > 0) {
            DrawTexturePro(currentTexture,
                          {0, 0, (float)currentTexture.width, (float)currentTexture.height},
                          buttonRect,
                          {0, 0}, 0.0f, WHITE);
        } else {
            //Draw an orange rectangle if texture fails to load
            Color buttonColor = (i == selectedButton) ? ORANGE : Color {255, 165, 0, 255};
            DrawRectangleRounded(buttonRect, 0.2f, 10, buttonColor);
        }
        
        // FIXED: Button text centering
        float buttonTextSpacing = 3.0f;
        Vector2 textSizeNoSpacing = MeasureTextEx(UI::font, buttonTexts[i].c_str(), 24, 0);
        float textX = buttonRect.x + (buttonRect.width-200) / 2;
        float textY = buttonRect.y + (buttonRect.height-24) / 2;
        
        // Text with shadow for better visibility
        DrawTextEx(UI::font, buttonTexts[i].c_str(), 
                  {textX + 1, textY + 1}, 20, buttonTextSpacing, GRAY);
        DrawTextEx(UI::font, buttonTexts[i].c_str(), 
                  {textX, textY}, 20, buttonTextSpacing, WHITE);

        if (i < buttonTextures.size() && buttonTextures[i].id > 0) {
            float padding    = 10.0f;
            float maxH       = buttonRect.height - padding*2;
            float scale      = maxH / buttonTextures[i].height;
            float iconW      = buttonTextures[i].width  * scale;
            float iconH      = buttonTextures[i].height * scale;
            float iconX      = buttonRect.x + buttonRect.width - iconW - padding;
            float iconY      = buttonRect.y + (buttonRect.height - iconH) * 0.5f;

            // Draw scaled icon
            DrawTexturePro(
                buttonTextures[i],
                { 0.0f, 0.0f, (float)buttonTextures[i].width, (float)buttonTextures[i].height }, // source
                { iconX, iconY, iconW, iconH },                                                   // dest
                { 0.0f, 0.0f }, 0.0f, WHITE
            );
        }
    }
    
    // Draw selection arrow
    float arrowY = startY + selectedButton * spacing + 30;
    DrawTriangle(
        { buttonX - 30, arrowY - 10 },
        { buttonX - 30, arrowY + 10 },
        { buttonX - 15, arrowY },
        YELLOW
    );
    
    float instrSpacing = 1.0f;
    float instrX = UI::screenWidth - 300;
    float instrY = UI::screenHeight - 200;

    DrawTextEx(UI::font, instructionsText.c_str(), 
              {instrX + 1, instrY + 1}, 16, instrSpacing, DARKGRAY);
    DrawTextEx(UI::font, instructionsText.c_str(), 
              {instrX, instrY}, 16, instrSpacing, LIGHTGRAY);
}