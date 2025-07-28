#include "Settings.hpp"
#include "raylib.h"
#include <cmath>
#include <vector>
#include <string>


static bool   buttonsInitialized = false;

Settings::Settings(SceneManager* _manager) : Scene(_manager) {
    // Load textures (reuse menu textures)
    buttonTexture = LoadTexture("assets/Backgrounds/Buttons/MenuButton.png");
    buttonHoverTexture = LoadTexture("assets/Backgrounds/Buttons/MenuButtonHovered.png");
    settingsBoard = LoadTexture("assets/Backgrounds/SettingsBoard.png");
    
    if (buttonTexture.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load settings button texture");
    }
    if (buttonHoverTexture.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load settings button hover texture");
    }
    if (settingsBoard.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load settings board texture");
    }
}

Settings::~Settings() {
    if (buttonTexture.id > 0) {
        UnloadTexture(buttonTexture);
    }
    if (buttonHoverTexture.id > 0) {
        UnloadTexture(buttonHoverTexture);
    }
    if (settingsBoard.id > 0) {
        UnloadTexture(settingsBoard);
    }
    
    for (auto button : buttons) {
        delete button;
    }
    buttons.clear();
}

void Settings::updateScene() {

    // Update switch animation continuously
    float targetPos = fullscreen ? 1.0f : 0.0f;
    float animSpeed = 8.0f;
    switchAnimation += (targetPos - switchAnimation) * animSpeed * GetFrameTime();


    // Initialize buttons once
    if (!buttonsInitialized) {
        std::vector<std::string> buttonTexts = {
            "MUSIC",
            "SFX", 
            "FULLSCREEN",
            "BACK"
        };
        
        for (int i = 0; i < buttonTexts.size(); i++) {
            buttons.push_back(new TextBox(buttonTexts[i], 0, 0, WHITE, ORANGE, BLACK));
        }
        
        buttonsInitialized = true;
    }
    
    // Navigation
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        selectedButton = (selectedButton - 1 + 4) % 4;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        selectedButton = (selectedButton + 1) % 4;
    }
    
    // Adjust settings with left/right arrows
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        switch(selectedButton) {
            case 0: musicVolume = fmaxf(0.0f, musicVolume - 0.1f); break;
            case 1: sfxVolume = fmaxf(0.0f, sfxVolume - 0.1f); break;
            case 2: {
                fullscreen = false; 
                break;
            }
        }
    }

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        switch(selectedButton) {
            case 0: musicVolume = fminf(1.0f, musicVolume + 0.1f); break;
            case 1: sfxVolume = fminf(1.0f, sfxVolume + 0.1f); break;
            case 2: {
                fullscreen = true; 
                break;
            }
        }
    }
    
    // Activate button
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        if (selectedButton == 2) {
            fullscreen = !fullscreen;
        } 
        else if (selectedButton == 3) { // Back to menu
            manager->goBack();
            return;
        }
    }

    // Mouse interaction
    Vector2 mousePos = GetMousePosition();
    float boardWidth = 600;
    float boardHeight = 525;
    float boardX = UI::screenWidth / 2 - boardWidth / 2;
    float boardY = UI::screenHeight / 2 - 350;
    float buttonWidth = 400;
    float buttonHeight = 80;
    float buttonX = boardX + (boardWidth - buttonWidth) / 2; // Center buttons in board
    float startY = boardY + 100; // Start buttons below board title area
    float spacing = 90;

    for (int i = 0; i < 4; ++i) {
        Rectangle btnRect = {
            (float)buttonX,
            (float)(startY + i * spacing),
            (float)buttonWidth,
            (float)buttonHeight
        };
        if (CheckCollisionPointRec(mousePos, btnRect)) {
            selectedButton = i;
            break;
        }
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (selectedButton == 0 || selectedButton == 1) {
            // Match the slider position from displayScene()
            Rectangle buttonRect = {buttonX, startY + selectedButton * spacing, buttonWidth, buttonHeight};
            float sliderY = buttonRect.y + buttonRect.height - 20;
            float sliderX = buttonRect.x + 20;
            float sliderWidth = buttonRect.width - 40;
            Rectangle sliderArea = {sliderX, sliderY - 5, sliderWidth, 18}; // Slightly larger hit area
            
            if (CheckCollisionPointRec(mousePos, sliderArea)) {
                float newValue = (mousePos.x - sliderX) / sliderWidth;
                newValue = fmaxf(0.0f, fminf(1.0f, newValue));
                
                if (selectedButton == 0) {
                    musicVolume = newValue;
                } else {
                    sfxVolume = newValue;
                }
            }
        }
        // Handle fullscreen toggle
        else if (selectedButton == 2) {
            Rectangle btnRect = {
                (float)buttonX,
                (float)(startY + selectedButton * spacing),
                (float)buttonWidth,
                (float)buttonHeight
            };
            if (CheckCollisionPointRec(mousePos, btnRect)) {
                fullscreen = !fullscreen;
                float targetPos = fullscreen ? 1.0f : 0.0f;
                float animSpeed = 8.0f;
                switchAnimation += (targetPos - switchAnimation) * animSpeed * GetFrameTime();
            }
        }
        // Handle back to menu
        else if (selectedButton == 3) {
            Rectangle btnRect = {
                (float)buttonX,
                (float)(startY + selectedButton * spacing),
                (float)buttonWidth,
                (float)buttonHeight
            };
            if (CheckCollisionPointRec(mousePos, btnRect)) {
                manager->goBack();
                return;
            }
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        manager->goBack();
    }
}

void Settings::displayScene() {
    std::string titleText = "SETTINGS";
    std::vector<std::string> buttonTexts = {
        "MUSIC",
        "SFX", 
        "FULLSCREEN",
        "BACK"
    };
    
    //Draw background
    DrawRectangle(0, 0, UI::screenWidth, UI::screenHeight, Color{0, 0, 0, 80}); // Semi-transparent black background
    
    
    // Calculate settings board dimensions and position
    float boardWidth = 600;
    float boardHeight = 525;
    float boardX = UI::screenWidth / 2 - boardWidth / 2;
    float boardY = UI::screenHeight / 2 - 350;
    Rectangle boardRect = {boardX, boardY, boardWidth, boardHeight};

    
    
    // Draw the settings board texture first (contains all buttons)
    if (settingsBoard.id > 0) {
        DrawTexturePro(settingsBoard,
                      {0, 0, (float)settingsBoard.width, (float)settingsBoard.height},
                      boardRect,
                      {0, 0}, 0.0f, WHITE);
    } else {
        // Fallback background for the board
        DrawRectangleRounded(boardRect, 0.1f, 10, Color{139, 69, 19, 255}); // Brown background
    }

    // Draw title
    float titleSpacing = 5.0f;
    float titleFontSize = 48.0f;
    Vector2 titleSize = MeasureTextEx(UI::boldFont, titleText.c_str(), titleFontSize, titleSpacing);
    float titleX = (UI::screenWidth - titleSize.x) / 2.0f;
    float titleY = boardY + 30;

    DrawTextEx(UI::boldFont, titleText.c_str(),
               { titleX + 2, titleY + 2 }, titleFontSize, titleSpacing, ORANGE);
    DrawTextEx(UI::boldFont, titleText.c_str(),
               { titleX, titleY }, titleFontSize, titleSpacing, BLACK);
    
    // Draw buttons inside the settings board
    float buttonWidth = 400;
    float buttonHeight = 80;
    float buttonX = boardX + (boardWidth - buttonWidth) / 2; // Center buttons in board
    float startY = boardY + 100; // Start buttons below board title area
    float spacing = 90;

     for (int i = 0; i < 4; i++) {
        Rectangle buttonRect = {buttonX, startY + i * spacing, buttonWidth, buttonHeight};
        
        // Only draw selection highlight, no background (the board is the background)
        if (i == selectedButton) {
            // Draw selection highlight
            DrawRectangleRoundedLines(buttonRect, 0.2f, 10, ORANGE);
            DrawRectangleRounded(buttonRect, 0.2f, 10, Color{255, 165, 0, 50});
        }
        
        // Button text
        float buttonTextSpacing = 3.0f;
        float textX = buttonRect.x + 20; // Left aligned
        float textY = buttonRect.y + (buttonRect.height - 24) / 2;
        
        // Special handling for "BACK TO MENU" button - center it and add outline
        if (i == 3) {
            
            // Center the text for back to menu button
            Vector2 textSize = MeasureTextEx(UI::font, buttonTexts[i].c_str(), 24, buttonTextSpacing);
            textX = buttonRect.x + (buttonRect.width - textSize.x) / 2;
            
            // Draw outline
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx != 0 || dy != 0) {
                        DrawTextEx(UI::font, buttonTexts[i].c_str(), 
                                  {textX + dx, textY + dy}, 24, buttonTextSpacing, BLACK);
                    }
                }
            }
        } else {
            // Text with shadow for other buttons
            DrawTextEx(UI::font, buttonTexts[i].c_str(), 
                      {textX + 1, textY + 1}, 24, buttonTextSpacing, WHITE);
        }
        
        DrawTextEx(UI::font, buttonTexts[i].c_str(), 
                  {textX, textY}, 24, buttonTextSpacing, BLACK);
        

        // Draw setting values and sliders
        switch(i) {
            case 0: // Music Volume
            case 1: { // SFX Volume
                float volume = (i == 0) ? musicVolume : sfxVolume;
                
                // Position slider below the button text
                float sliderY = buttonRect.y + buttonRect.height - 20;
                float sliderX = buttonRect.x + 20;
                float sliderWidth = buttonRect.width - 40;
                
                // Draw slider background
                Rectangle sliderBg = {sliderX, sliderY, sliderWidth, 8};
                DrawRectangleRounded(sliderBg, 0.5f, 10, Color{101, 67, 33, 255}); // Dark brown
                
                // Draw slider fill
                Rectangle sliderFill = {sliderX, sliderY, sliderWidth * volume, 8};
                Color fillColor = (i == selectedButton) ? ORANGE : Color{160, 82, 45, 255}; // Saddle brown
                DrawRectangleRounded(sliderFill, 0.5f, 10, fillColor);
                
                // Draw slider handle
                float handleX = sliderX + (sliderWidth * volume) - 6;
                Rectangle handle = {handleX, sliderY - 3, 12, 14};
                Color handleColor = (i == selectedButton) ? WHITE : Color{205, 133, 63, 255}; // Peru
                DrawRectangleRounded(handle, 0.3f, 10, handleColor);
                
                // Draw percentage text next to slider
                std::string valueText = std::to_string((int)(volume * 100)) + "%";
                float percentX = sliderX + sliderWidth + 10;
                DrawTextEx(UI::font, valueText.c_str(), 
                          {percentX, sliderY - 5}, 20, 2, Color{139, 69, 19, 255});
                break;
            }
            case 2: // Fullscreen toggle
            {
                float valueX = buttonRect.x + buttonRect.width - 150; // Moved left to accommodate larger size
                Rectangle switchBg = {valueX, textY + 4, 80, 24}; // Increased from 50x16 to 80x24
                Color bgColor = fullscreen ? ORANGE: Color{101, 67, 33, 255}; //Dark brown
                DrawRectangleRounded(switchBg, 0.5f, 10, bgColor);
                
                // Switch border (when selected)
                if (i == selectedButton) {
                    DrawRectangleRoundedLines(switchBg, 0.5f, 10, WHITE);
                }
                
                // Animated switch handle
                float animatedX = valueX + 4 + (48.0f * switchAnimation);
                Rectangle handle = {animatedX, textY + 8, 16, 16};
                DrawRectangleRounded(handle, 0.5f, 10, WHITE);
                
                // Switch label
                std::string valueText = fullscreen ? "ON" : "OFF";
                DrawTextEx(UI::font, valueText.c_str(), 
                          {valueX + 90, textY + 5}, 20, 2, Color{139, 69, 19, 255}); 
                break;
            }
            case 3:
                // No additional elements for back to menu
                break;
        }
    }
    
    
    // Draw instructions
    std::string instructions = "Use ARROW KEYS to navigate and adjust settings\nENTER to select - ESC to go back";
    float instrX = UI::screenWidth - 550;
    float instrY = UI::screenHeight - 150;
    
    DrawTextEx(UI::font, instructions.c_str(), 
              {instrX + 1, instrY + 1}, 20, 2, DARKGRAY);
    DrawTextEx(UI::font, instructions.c_str(), 
              {instrX, instrY}, 20, 2, WHITE);
}