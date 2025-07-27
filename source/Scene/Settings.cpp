#include "Settings.hpp"
#include "raylib.h"
#include <cmath>
#include <vector>
#include <string>

// Static variables for flash effect
static bool        flashActive    = false;
static float       flashTimer     = 0.0f;
static sceneType   nextScene      = sceneType::MENU;
static const float flashDuration  = 0.6f;
static bool        buttonsInitialized = false;

Settings::Settings(SceneManager* _manager) : Scene(_manager) {
    // Load textures (reuse menu textures)
    backgroundTexture = LoadTexture("assets/Backgrounds/MenuBackground.png");
    buttonTexture = LoadTexture("assets/Backgrounds/Buttons/MenuButton.png");
    buttonHoverTexture = LoadTexture("assets/Backgrounds/Buttons/MenuButtonHovered.png");
    selectionIconTexture = LoadTexture("assets/Backgrounds/Buttons/SelectionIcon.png");
    
    if (backgroundTexture.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load settings background texture");
    }
    if (buttonTexture.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load settings button texture");
    }
    if (buttonHoverTexture.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load settings button hover texture");
    }
    if (selectionIconTexture.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load settings selection icon texture");
    }
}

Settings::~Settings() {
    if (backgroundTexture.id > 0) {
        UnloadTexture(backgroundTexture);
    }
    if (buttonTexture.id > 0) {
        UnloadTexture(buttonTexture);
    }
    if (buttonHoverTexture.id > 0) {
        UnloadTexture(buttonHoverTexture);
    }
    if (selectionIconTexture.id > 0) {
        UnloadTexture(selectionIconTexture);
    }
    
    for (auto button : buttons) {
        delete button;
    }
    buttons.clear();
}

void Settings::updateScene() {
    // Handle flash effect
    if (flashActive) {
        flashTimer += GetFrameTime();
        if (flashTimer >= flashDuration) {
            flashActive = false;
            flashTimer = 0.0f;
            manager->changeScene(nextScene);
        }
        return;
    }

    // Initialize buttons once
    if (!buttonsInitialized) {
        std::vector<std::string> buttonTexts = {
            "MUSIC VOLUME",
            "SFX VOLUME", 
            "FULLSCREEN",
            "BACK TO MENU"
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
                float targetPos = fullscreen ? 1.0f : 0.0f;
                float animSpeed = 8.0f;
                switchAnimation += (targetPos - switchAnimation) * animSpeed * GetFrameTime();
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
                float targetPos = fullscreen ? 1.0f : 0.0f;
                float animSpeed = 8.0f;
                switchAnimation += (targetPos - switchAnimation) * animSpeed * GetFrameTime();
                break;
            }
        }
    }
    
    // Activate button
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        if (selectedButton == 2) {
            fullscreen = !fullscreen;
            float targetPos = fullscreen ? 1.0f : 0.0f;
            float animSpeed = 8.0f;
            switchAnimation += (targetPos - switchAnimation) * animSpeed * GetFrameTime();
        } 
        else if (selectedButton == 3) { // Back to menu
            nextScene = sceneType::MENU;
            flashActive = true;
            flashTimer = 0.0f;
            return;
        }
    }

    // Mouse interaction
    Vector2 mousePos = GetMousePosition();
    int buttonWidth = 300;
    int buttonHeight = 60;
    int buttonX = UI::screenWidth / 2 - buttonWidth / 2;
    int startY = UI::screenHeight / 2 - 80;
    int spacing = 80;

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
            float valueX = buttonX + 300 - 150;
            float sliderY = startY + selectedButton * spacing + 30 + 5;
            Rectangle sliderArea = {valueX, sliderY, 100, 10};
            
            if (CheckCollisionPointRec(mousePos, sliderArea)) {
                float newValue = (mousePos.x - valueX) / 100.0f;
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
                nextScene = sceneType::MENU;
                flashActive = true;
                flashTimer = 0.0f;
                return;
            }
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        manager->changeScene(sceneType::MENU);
    }
}

void Settings::displayScene() {
    std::string titleText = "SETTINGS";
    std::vector<std::string> buttonTexts = {
        "MUSIC VOLUME",
        "SFX VOLUME", 
        "FULLSCREEN",
        "BACK TO MENU"
    };
    
    // Draw background
    if (backgroundTexture.id > 0) {
        DrawTexturePro(backgroundTexture, 
                      {0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
                      {0, 0, (float)UI::screenWidth, (float)UI::screenHeight},
                      {0, 0}, 0.0f, WHITE);
    } else {
        ClearBackground(BLACK);
    }
    
    // Draw title
    float titleSpacing = 12.0f;
    float titleFontSize = 72.0f;
    Vector2 titleSize = MeasureTextEx(UI::boldFont, titleText.c_str(), titleFontSize, titleSpacing);
    float titleX = (UI::screenWidth - titleSize.x) / 2.0f;
    float titleY = 80;

    DrawTextEx(UI::boldFont, titleText.c_str(),
               { titleX + 2, titleY + 2 }, titleFontSize, titleSpacing, ORANGE);
    DrawTextEx(UI::boldFont, titleText.c_str(),
               { titleX, titleY }, titleFontSize, titleSpacing, RED);
    
    // Draw buttons and settings
    float buttonWidth = 300;
    float buttonHeight = 60;
    float buttonX = UI::screenWidth / 2 - buttonWidth / 2;
    float startY = UI::screenHeight / 2 - 80;
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
            Color buttonColor = (i == selectedButton) ? ORANGE : Color {255, 165, 0, 255};
            DrawRectangleRounded(buttonRect, 0.2f, 10, buttonColor);
        }
        
        // Button text
        float buttonTextSpacing = 3.0f;
        Vector2 textSize = MeasureTextEx(UI::font, buttonTexts[i].c_str(), 20, buttonTextSpacing);
        float textX = buttonRect.x + 20; // Left aligned
        float textY = buttonRect.y + (buttonRect.height - textSize.y) / 2;
        
        // Text with shadow
        DrawTextEx(UI::font, buttonTexts[i].c_str(), 
                  {textX + 1, textY + 1}, 20, buttonTextSpacing, BLACK);
        
        // Flash effect for selected button
        Color mainColor = WHITE;
        if (flashActive && i == selectedButton) {
            float alpha = 1.0f - (flashTimer / flashDuration);
            mainColor.a = (unsigned char)(alpha * 255);
        }
        DrawTextEx(UI::font, buttonTexts[i].c_str(), 
                  {textX, textY}, 20, buttonTextSpacing, mainColor);
        
        // Draw setting values
        float valueX = buttonRect.x + buttonRect.width - 150;
        std::string valueText;
        
        switch(i) {
            case 0: // Music Volume Slider
            case 1: { // SFX Volume Slider
                float volume = (i == 0) ? musicVolume : sfxVolume;
                
                // Draw slider background
                Rectangle sliderBg = {valueX, textY + 5, 100, 10};
                DrawRectangleRounded(sliderBg, 0.5f, 10, DARKGRAY);
                
                // Draw slider fill
                Rectangle sliderFill = {valueX, textY + 5, 100 * volume, 10};
                Color fillColor = (i == selectedButton) ? ORANGE : LIGHTGRAY;
                DrawRectangleRounded(sliderFill, 0.5f, 10, fillColor);
                
                // Draw slider handle
                float handleX = valueX + (100 * volume) - 5;
                Rectangle handle = {handleX, textY + 2, 10, 16};
                Color handleColor = (i == selectedButton) ? WHITE : GRAY;
                DrawRectangleRounded(handle, 0.3f, 10, handleColor);
                
                // Draw percentage text
                valueText = std::to_string((int)(volume * 100)) + "%";
                DrawTextEx(UI::font, valueText.c_str(), 
                          {valueX + 110, textY}, 16, 2, WHITE);
                break;
            }
            case 2: // Fullscreen toggle
            {
                Rectangle switchBg = {valueX, textY + 5, 60, 20};
                Color bgColor = fullscreen ? GREEN : DARKGRAY;
                DrawRectangleRounded(switchBg, 0.5f, 10, bgColor);
                
                // Switch border (when selected)
                if (i == selectedButton) {
                    DrawRectangleRoundedLines(switchBg, 0.5f, 10, WHITE);
                }
                
                // Animated switch handle
                float animatedX = valueX + 5 + (35.0f * switchAnimation);
                Rectangle handle = {animatedX, textY + 7, 16, 16};
                DrawRectangleRounded(handle, 0.5f, 10, WHITE);
                
                // Switch label
                valueText = fullscreen ? "ON" : "OFF";
                DrawTextEx(UI::font, valueText.c_str(), 
                          {valueX + 70, textY}, 16, 2, WHITE);
                break;
            }
            case 3:
                break;
        }
    }
    
    // Draw selection icon
    if (selectionIconTexture.id > 0) {
        float iconScale = 0.15f;
        float iconW = selectionIconTexture.width * iconScale;
        float iconH = selectionIconTexture.height * iconScale;
        float iconX = buttonX - iconW - 10;
        float iconY = startY + selectedButton * spacing + (buttonHeight - iconH) * 0.5f;

        DrawTexturePro(selectionIconTexture,
                      {0, 0, (float)selectionIconTexture.width, (float)selectionIconTexture.height},
                      {iconX, iconY, iconW, iconH},
                      {0, 0}, 0.0f, WHITE);
    }
    
    // Draw instructions
    std::string instructions = "Use ARROW KEYS to navigate and adjust settings\nENTER to select • ESC to go back";
    float instrX = UI::screenWidth / 2 - 200;
    float instrY = UI::screenHeight - 100;
    
    DrawTextEx(UI::font, instructions.c_str(), 
              {instrX + 1, instrY + 1}, 16, 2, DARKGRAY);
    DrawTextEx(UI::font, instructions.c_str(), 
              {instrX, instrY}, 16, 2, WHITE);
}