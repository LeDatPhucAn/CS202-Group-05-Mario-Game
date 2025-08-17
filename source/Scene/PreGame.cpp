#include "PreGame.hpp"
#include "raylib.h"
#include <cmath>
#include "SoundController.hpp"
#include "GameInfo.hpp"

static float logoBounce = 0.0f;

PreGame::PreGame(SceneManager* _manager) : Scene(_manager)
{
    // Load textures
    backgroundTexture = LoadTexture("assets/Backgrounds/PregameBackground.png");
    marioTexture = LoadTexture("assets/Backgrounds/Mario.png");
    luigiTexture = LoadTexture("assets/Backgrounds/Luigi.png");
    dualTexture = LoadTexture("assets/Backgrounds/Dual.png");

    if (backgroundTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load PreGame background texture");
    }
    if (marioTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load PreGame Mario texture");
    }
    if (luigiTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load PreGame Luigi texture");
    }
    if (dualTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load PreGame Dual texture");
    }

    initializeDifficultyButtons();
    
    // Set initial selections based on GameInfo defaults
    GameInfo* gameInfo = GameInfo::getInstance();
    if (gameInfo->getMode() == Mode::DUAL) {
        selectedModeIndex = 2;
    } else if (gameInfo->isMario()) {
        selectedModeIndex = 0;
    } else {
        selectedModeIndex = 1;
    }
    
    switch(gameInfo->getDifficulty()) {
        case Difficulty::EASY: selectedDifficultyIndex = 0; break;
        case Difficulty::HARD: selectedDifficultyIndex = 1; break;
        case Difficulty::HARDCORE: selectedDifficultyIndex = 2; break;
    }
    
    // Play menu music
    SoundController::getInstance().playSceneMusic(sceneType::MENU);
}

PreGame::~PreGame()
{
    if (backgroundTexture.id > 0)
    {
        UnloadTexture(backgroundTexture);
        backgroundTexture.id = 0;
    }
    if (marioTexture.id > 0)
    {
        UnloadTexture(marioTexture);
        marioTexture.id = 0;
    }
    if (luigiTexture.id > 0)
    {
        UnloadTexture(luigiTexture);
        luigiTexture.id = 0;
    }
    if (dualTexture.id > 0)
    {
        UnloadTexture(dualTexture);
        dualTexture.id = 0;
    }

    for (auto button : difficultyButtons)
    {
        delete button;
    }
    difficultyButtons.clear();
}

void PreGame::initializeDifficultyButtons()
{
    float buttonWidth = 200;
    float buttonHeight = 50;
    float startY = UI::screenHeight / 2 + 50;
    float horizontalSpacing = 220;
    
    float centerX = UI::screenWidth / 2 ;
    
    // Difficulty Easy (Button 0)
    TextBox* easyButton = new TextBox(
        "EASY",
        centerX - horizontalSpacing,
        startY,
        WHITE,
        Color{70, 130, 180, 255}, // Steel blue
        DARKGRAY
    );
    difficultyButtons.push_back(easyButton);
    
    // Difficulty Hard (Button 1)
    TextBox* hardButton = new TextBox(
        "HARD",
        centerX - buttonWidth / 2,
        startY,
        WHITE,
        Color{70, 130, 180, 255}, // Steel blue
        DARKGRAY
    );
    difficultyButtons.push_back(hardButton);
    
    // Difficulty Hardcore (Button 2)
    TextBox* hardcoreButton = new TextBox(
        "HARDCORE",
        centerX + horizontalSpacing - buttonWidth,
        startY,
        WHITE,
        Color{70, 130, 180, 255}, // Steel blue
        DARKGRAY
    );
    difficultyButtons.push_back(hardcoreButton);
    
    // Play button
    TextBox* playButton = new TextBox(
        "PLAY",
        centerX - 140,
        startY + 80,
        WHITE,
        Color{0, 150, 0, 255}, // Green
        DARKGRAY
    );
    difficultyButtons.push_back(playButton);
    
    // Back button
    TextBox* backButton = new TextBox(
        "BACK",
        centerX,
        startY + 80,
        WHITE,
        Color{70, 130, 180, 255}, // Steel blue
        DARKGRAY
    );
    difficultyButtons.push_back(backButton);
}

void PreGame::handleInput()
{
    // Navigate through mode images (0-2) and difficulty buttons (0-4)
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
    {
        if (isSelectingMode && selectedModeIndex < 2) // Mode images (0, 1, 2)
        {
            selectedModeIndex++;
        }
        else if (!isSelectingMode && selectedDifficultyIndex < 4) // Difficulty buttons (0-4)
        {
            selectedDifficultyIndex++;
        }
    }
    
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
    {
        if (isSelectingMode && selectedModeIndex > 0) // Mode images (0, 1, 2)
        {
            selectedModeIndex--;
        }
        else if (!isSelectingMode && selectedDifficultyIndex > 0) // Difficulty buttons (0-4)
        {
            selectedDifficultyIndex--;
        }
    }
    
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        // Switch focus from mode images to difficulty buttons
        isSelectingMode = false;
    }
    
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        // Switch focus from difficulty buttons to mode images
        isSelectingMode = true;
    }
    
    // Handle Enter key press
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        if (isSelectingMode) // Mode selection
        {
            if (selectedModeIndex == 0) // Single Player Mario
            {
                GameInfo::getInstance()->setMode(Mode::SINGLE);
                GameInfo::getInstance()->setCharacter(true);
            }
            else if (selectedModeIndex == 1) // Single Player Luigi
            {
                GameInfo::getInstance()->setMode(Mode::SINGLE);
                GameInfo::getInstance()->setCharacter(false);
            }
            else if (selectedModeIndex == 2) // Two Player
            {
                GameInfo::getInstance()->setMode(Mode::DUAL);
            }
        }
        else // Difficulty/action button selection
        {
            if (selectedDifficultyIndex == 0) // Easy
            {
                GameInfo::getInstance()->setDifficulty(Difficulty::EASY);
            }
            else if (selectedDifficultyIndex == 1) // Hard
            {
                GameInfo::getInstance()->setDifficulty(Difficulty::HARD);
            }
            else if (selectedDifficultyIndex == 2) // Hardcore
            {
                GameInfo::getInstance()->setDifficulty(Difficulty::HARDCORE);
            }
            else if (selectedDifficultyIndex == 3) // Play button
            {
                // Start the game with selected mode and difficulty
                manager->changeScene(sceneType::GAME);
            }
            else if (selectedDifficultyIndex == 4) // Back button
            {
                manager->changeScene(sceneType::MENU);
            }
        }
    }
    
    // Mouse interaction with difficulty buttons only
    Vector2 mousePos = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    
    for (int i = 0; i < difficultyButtons.size(); i++)
    {
        Rectangle buttonRect = {
            difficultyButtons[i]->rect.x,
            difficultyButtons[i]->rect.y,
            difficultyButtons[i]->rect.width,
            difficultyButtons[i]->rect.height
        };
        
        if (CheckCollisionPointRec(mousePos, buttonRect))
        {
            isSelectingMode = false; // Switch to difficulty selection
            selectedDifficultyIndex = i;
            
            if (mouseClicked)
            {
                if (i == 0) // Easy
                {
                    GameInfo::getInstance()->setDifficulty(Difficulty::EASY);
                }
                else if (i == 1) // Hard
                {
                    GameInfo::getInstance()->setDifficulty(Difficulty::HARD);
                }
                else if (i == 2) // Hardcore
                {
                    GameInfo::getInstance()->setDifficulty(Difficulty::HARDCORE);
                }
                else if (i == 3) // Play button
                {
                    manager->changeScene(sceneType::GAME);
                }
                else if (i == 4) // Back button
                {
                    manager->changeScene(sceneType::MENU);
                }
            }
        }
    }
    
    // Mouse interaction with mode images
    float modeImageWidth = 150;
    float modeImageHeight = 200;
    float modeStartX = UI::screenWidth / 2 - 350;
    float modeImageY = 150.0f;
    float modeSpacing = 250;
    
    for (int i = 0; i < 3; i++)
    {
        Rectangle imageRect = {
            modeStartX + i * modeSpacing,
            modeImageY,
            modeImageWidth,
            modeImageHeight
        };
        
        if (CheckCollisionPointRec(mousePos, imageRect))
        {
            isSelectingMode = true; // Switch to mode selection
            selectedModeIndex = i;
            
            if (mouseClicked)
            {
                if (i == 0) // Single Player Mario
                {
                    GameInfo::getInstance()->setMode(Mode::SINGLE);
                    GameInfo::getInstance()->setCharacter(true);
                }
                else if (i == 1) // Single Player Luigi
                {
                    GameInfo::getInstance()->setMode(Mode::SINGLE);
                    GameInfo::getInstance()->setCharacter(false);
                }
                else if (i == 2) // Two Player
                {
                    GameInfo::getInstance()->setMode(Mode::DUAL);
                }
            }
        }
    }
    
    // Back with escape or backspace
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE))
    {
        manager->changeScene(sceneType::MENU);
    }
}

void PreGame::updateAnimations()
{
    animationTimer += GetFrameTime();
    
    // No logo animation needed anymore
}

void PreGame::updateScene()
{
    handleInput();
    updateAnimations();
    
    // Update difficulty button hover states
    for (int i = 0; i < difficultyButtons.size(); i++)
    {
        difficultyButtons[i]->isHovered = (!isSelectingMode && i == selectedDifficultyIndex);
        
        // Update button colors based on hover/selected state
        if (!isSelectingMode && i == selectedDifficultyIndex)
        {
            difficultyButtons[i]->FillColor = Color{255, 165, 0, 255}; // Orange when selected
        }
        else
        {
            // Different colors for different buttons
            if (i == 3) // Play button
            {
                difficultyButtons[i]->FillColor = Color{0, 150, 0, 255}; // Green
            }
            else
            {
                difficultyButtons[i]->FillColor = Color{70, 130, 180, 255}; // Blue
            }
        }
    }
}

void PreGame::displayScene()
{   
    // Draw background
    if (backgroundTexture.id > 0)
    {
        Rectangle srcRect = {0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height};
        Rectangle destRect = {0, 0, (float)UI::screenWidth, (float)UI::screenHeight};
        DrawTexturePro(backgroundTexture, srcRect, destRect, {0, 0}, 0.0f, WHITE);
    }
    else
    {
        DrawRectangle(0, 0, UI::screenWidth, UI::screenHeight, Color{30, 30, 60, 255});
    }
    DrawRectangle(0, 0, UI::screenWidth, UI::screenHeight, Color{0, 0, 0, 50});
    
    // Draw title text
    std::string titleText = "SELECT GAME MODE & DIFFICULTY";
    float titleFontSize = 32.0f;
    float titleSpacing = 3.0f;
    float titleX = UI::screenWidth / 2.0f - 300;
    float titleY = 100.0f;
    
    // Title shadow
    DrawTextEx(UI::boldFont, titleText.c_str(),
               {titleX + 2, titleY + 2}, titleFontSize, titleSpacing, Color{0, 0, 0, 150});
    // Title text
    DrawTextEx(UI::boldFont, titleText.c_str(),
               {titleX, titleY}, titleFontSize, titleSpacing, Color{255, 255, 255, 255});
    
    // Draw mode selection images
    float modeImageWidth = 150;
    float modeImageHeight = 200;
    float modeStartX = UI::screenWidth / 2 - 350;
    float modeImageY = 150.0f;
    float modeSpacing = 250;
    
    // Game mode images
    Texture2D modeTextures[3] = {marioTexture, luigiTexture, dualTexture};
    std::string modeLabels[3] = {"MARIO", "LUIGI", "DUAL PLAYER"};
    
    for (int i = 0; i < 3; i++)
    {
        float imageX = modeStartX + i * modeSpacing;
        float imageY = modeImageY;
        
        // Calculate scale and zoom effect for selected mode
        float scale = 1.0f;
        if (isSelectingMode && i == selectedModeIndex)
        {
            scale = 1.2f; 
        }
        
        float scaledWidth = modeImageWidth * scale;
        float scaledHeight = modeImageHeight * scale;
        float adjustedX = imageX - (scaledWidth - modeImageWidth) / 2.0f;
        float adjustedY = imageY - (scaledHeight - modeImageHeight) / 2.0f;
        
        // Draw mode image
        if (modeTextures[i].id > 0)
        {
            Rectangle srcRect = {0, 0, (float)modeTextures[i].width, (float)modeTextures[i].height};
            Rectangle destRect = {adjustedX, adjustedY, scaledWidth, scaledHeight};
            DrawTexturePro(modeTextures[i], srcRect, destRect, {0, 0}, 0.0f, WHITE);
        }
        
        // Draw mode label with color based on selection
        Color textColor = BLACK;
        GameInfo* gameInfo = GameInfo::getInstance();
        
        // Check if this mode is currently selected in GameInfo
        bool isCurrentMode = false;
        if (i == 0 && gameInfo->getMode() == Mode::SINGLE && gameInfo->isMario()) {
            isCurrentMode = true;
        } else if (i == 1 && gameInfo->getMode() == Mode::SINGLE && !gameInfo->isMario()) {
            isCurrentMode = true;
        } else if (i == 2 && gameInfo->getMode() == Mode::DUAL) {
            isCurrentMode = true;
        }
        
        if (isSelectingMode && i == selectedModeIndex)
        {
            textColor = Color{255, 255, 0, 255}; // Yellow for currently navigated
        }
        else if (isCurrentMode)
        {
            textColor = Color{255, 165, 0, 255}; // Orange for selected mode
        }
        
        float labelFontSize = 24.0f;
        Vector2 labelSize = MeasureTextEx(UI::font, modeLabels[i].c_str(), labelFontSize, 1);
        float labelX = imageX + (modeImageWidth - labelSize.x) / 2.0f;
        float labelY = imageY + modeImageHeight + 30;
        
        // Label shadow
        DrawTextEx(UI::font, modeLabels[i].c_str(),
                   {labelX + 1, labelY + 1}, labelFontSize, 1, Color{0, 0, 0, 150});
        // Label text
        DrawTextEx(UI::font, modeLabels[i].c_str(),
                   {labelX, labelY}, labelFontSize, 1, textColor);
    }
    
    // Draw difficulty buttons
    for (int i = 0; i < difficultyButtons.size(); i++)
    {
        // Check if this button represents the current difficulty
        bool isCurrentDifficulty = false;
        GameInfo* gameInfo = GameInfo::getInstance();
        
        if (i == 0 && gameInfo->getDifficulty() == Difficulty::EASY) {
            isCurrentDifficulty = true;
        } else if (i == 1 && gameInfo->getDifficulty() == Difficulty::HARD) {
            isCurrentDifficulty = true;
        } else if (i == 2 && gameInfo->getDifficulty() == Difficulty::HARDCORE) {
            isCurrentDifficulty = true;
        }
        
        // Set button colors based on state
        Color fillColor;
        if (isCurrentDifficulty) {
            fillColor = Color{255, 165, 0, 255}; // Orange for chosen difficulty
        } else if (i == 3) { // Play button
            fillColor = Color{0, 150, 0, 255}; // Green for play button
        } else {
            fillColor = Color{70, 130, 180, 255}; // Steel blue for default
        }
        
        // Override the button's fill color temporarily
        Color originalFillColor = difficultyButtons[i]->FillColor;
        difficultyButtons[i]->FillColor = fillColor;
        
        difficultyButtons[i]->draw();
        
        // Restore original color
        difficultyButtons[i]->FillColor = originalFillColor;
        
        // Add selection highlight for currently navigated button (yellow border)
        if (!isSelectingMode && i == selectedDifficultyIndex)
        {
            Rectangle buttonRect = {
                difficultyButtons[i]->rect.x - 5,
                difficultyButtons[i]->rect.y - 5,
                difficultyButtons[i]->rect.width + 10,
                difficultyButtons[i]->rect.height + 10
            };
            DrawRectangleRoundedLines(buttonRect, 0.1f, 8, Color{255, 255, 0, 200});
        }
    }
}