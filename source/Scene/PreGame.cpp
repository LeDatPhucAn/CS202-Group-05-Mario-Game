#include "PreGame.hpp"
#include "raylib.h"
#include <cmath>
#include "SoundController.hpp"
#include "GameInfo.hpp"

static float logoBounce = 0.0f;

PreGame::PreGame(SceneManager* _manager) : Scene(_manager)
{
    // Load textures
    backgroundTexture = LoadTexture("assets/Backgrounds/MenuBackground.png");
    logoTexture = LoadTexture("assets/Backgrounds/Title.png");
    
    if (backgroundTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load PreGame background texture");
    }
    if (logoTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load PreGame logo texture");
    }
    
    initializeButtons();
    
    // Play menu music
    SoundController::getInstance().playSceneMusicFromStart(sceneType::MENU);
}

PreGame::~PreGame()
{
    if (backgroundTexture.id > 0)
    {
        UnloadTexture(backgroundTexture);
        backgroundTexture.id = 0;
    }
    if (logoTexture.id > 0)
    {
        UnloadTexture(logoTexture);
        logoTexture.id = 0;
    }
    
    for (auto button : buttons)
    {
        delete button;
    }
    buttons.clear();
}

void PreGame::initializeButtons()
{
    float buttonWidth = 450;  // Made smaller to fit 3 in a row
    float buttonHeight = 60;
    float startY = UI::screenHeight / 2 + 80;
    float horizontalSpacing = 650;  // Increased spacing between buttons horizontally
    float verticalSpacing = 80;     // Space between rows
    
    // First row: Game Mode buttons (0, 1, 2)
    float firstRowY = startY;
    float centerX = UI::screenWidth / 2;
    
    // Single Player Mario (Button 0) - Left
    TextBox* singleMarioButton = new TextBox(
        "SINGLE PLAYER (MARIO)",
        centerX - horizontalSpacing,
        firstRowY,
        WHITE,
        Color{70, 130, 180, 255}, // Steel blue
        DARKGRAY
    );
    buttons.push_back(singleMarioButton);
    
    // Single Player Luigi (Button 1) - Center
    TextBox* singleLuigiButton = new TextBox(
        "SINGLE PLAYER (LUIGI)",
        centerX - buttonWidth / 2,
        firstRowY,
        WHITE,
        Color{70, 130, 180, 255}, // Steel blue
        DARKGRAY
    );
    buttons.push_back(singleLuigiButton);
    
    // Two Player (Button 2) - Right
    TextBox* twoPlayerButton = new TextBox(
        "TWO PLAYER",
        centerX + horizontalSpacing - buttonWidth,
        firstRowY,
        WHITE,
        Color{70, 130, 180, 255}, // Steel blue
        DARKGRAY
    );
    buttons.push_back(twoPlayerButton);
    
    // Second row: Difficulty buttons (3, 4, 5)
    float secondRowY = firstRowY + verticalSpacing;
    
    // Difficulty Easy (Button 3) - Left
    TextBox* easyButton = new TextBox(
        "DIFFICULTY: EASY",
        centerX - horizontalSpacing,
        secondRowY,
        WHITE,
        Color{70, 130, 180, 255}, // Steel blue
        DARKGRAY
    );
    buttons.push_back(easyButton);
    
    // Difficulty Hard (Button 4) - Center
    TextBox* hardButton = new TextBox(
        "DIFFICULTY: HARD",
        centerX - buttonWidth / 2,
        secondRowY,
        WHITE,
        Color{70, 130, 180, 255}, // Steel blue
        DARKGRAY
    );
    buttons.push_back(hardButton);
    
    // Difficulty Hardcore (Button 5) - Right
    TextBox* hardcoreButton = new TextBox(
        "DIFFICULTY: HARDCORE",
        centerX + horizontalSpacing - buttonWidth,
        secondRowY,
        WHITE,
        Color{70, 130, 180, 255}, // Steel blue
        DARKGRAY
    );
    buttons.push_back(hardcoreButton);
    
    // Third row: Play and Back buttons (6, 7)
    float thirdRowY = secondRowY + verticalSpacing;
    float playBackSpacing = 180;  // Space between Play and Back buttons
    
    // Play button (Button 6) - Left of center
    TextBox* playButton = new TextBox(
        "PLAY",
        centerX - playBackSpacing / 2 - buttonWidth,
        thirdRowY,
        WHITE,
        Color{0, 150, 0, 255},  // Keep green for PLAY button (special)
        DARKGRAY
    );
    buttons.push_back(playButton);
    
    // Back button (Button 7) - Right of center
    TextBox* backButton = new TextBox(
        "BACK",
        centerX + playBackSpacing / 2,
        thirdRowY,
        WHITE,
        Color{70, 130, 180, 255}, // Steel blue
        DARKGRAY
    );
    buttons.push_back(backButton);
}

void PreGame::handleInput()
{
    // 2D Grid navigation
    // Row 0: buttons 0, 1, 2 (Game modes)
    // Row 1: buttons 3, 4, 5 (Difficulties)
    // Row 2: buttons 6, 7 (Play, Back)
    
    int oldSelection = selectedButton;
    
    // Keyboard navigation with 2D grid logic
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        if (selectedButton >= 0 && selectedButton <= 2) {
            // From first row, wrap to third row
            selectedButton = (selectedButton <= 1) ? 6 : 7;  // Left/center -> Play, right -> Back
        } else if (selectedButton >= 3 && selectedButton <= 5) {
            // From second row to first row (same column)
            selectedButton = selectedButton - 3;
        } else if (selectedButton == 6 || selectedButton == 7) {
            // From third row to second row
            selectedButton = (selectedButton == 6) ? 3 : 5;  // Play -> Easy, Back -> Hardcore
        }
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        if (selectedButton >= 0 && selectedButton <= 2) {
            // From first row to second row (same column)
            selectedButton = selectedButton + 3;
        } else if (selectedButton >= 3 && selectedButton <= 5) {
            // From second row to third row
            selectedButton = (selectedButton <= 4) ? 6 : 7;  // Easy/Hard -> Play, Hardcore -> Back
        } else if (selectedButton == 6 || selectedButton == 7) {
            // From third row to first row
            selectedButton = (selectedButton == 6) ? 0 : 2;  // Play -> Mario, Back -> Two Player
        }
    }
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
    {
        if (selectedButton >= 0 && selectedButton <= 2) {
            // Navigate within first row
            selectedButton = (selectedButton == 0) ? 2 : selectedButton - 1;
        } else if (selectedButton >= 3 && selectedButton <= 5) {
            // Navigate within second row
            selectedButton = (selectedButton == 3) ? 5 : selectedButton - 1;
        } else if (selectedButton >= 6 && selectedButton <= 7) {
            // Navigate within third row
            selectedButton = (selectedButton == 6) ? 7 : 6;
        }
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
    {
        if (selectedButton >= 0 && selectedButton <= 2) {
            // Navigate within first row
            selectedButton = (selectedButton == 2) ? 0 : selectedButton + 1;
        } else if (selectedButton >= 3 && selectedButton <= 5) {
            // Navigate within second row
            selectedButton = (selectedButton == 5) ? 3 : selectedButton + 1;
        } else if (selectedButton >= 6 && selectedButton <= 7) {
            // Navigate within third row
            selectedButton = (selectedButton == 7) ? 6 : 7;
        }
    }
    
    
    // Selection
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        switch (selectedButton)
        {
            case 0: // Single Player Mario
                GameInfo::getInstance()->setMode(Mode::SINGLE);
                GameInfo::getInstance()->setCharacter(true);
                // Don't start game immediately, just set the mode
                break;
            case 1: // Single Player Luigi
                GameInfo::getInstance()->setMode(Mode::SINGLE);
                GameInfo::getInstance()->setCharacter(false);
                // Don't start game immediately, just set the mode
                break;
            case 2: // Two Player
                GameInfo::getInstance()->setMode(Mode::DUAL);
                // Don't start game immediately, just set the mode
                break;
            case 3: // Difficulty Easy
                GameInfo::getInstance()->setDifficulty(Difficulty::EASY);
                break;
            case 4: // Difficulty Hard
                GameInfo::getInstance()->setDifficulty(Difficulty::HARD);
                break;
            case 5: // Difficulty Hardcore
                GameInfo::getInstance()->setDifficulty(Difficulty::HARDCORE);
                break;
            case 6: // Play - Start the game
                manager->changeScene(sceneType::GAME);
                break;
            case 7: // Back
                manager->changeScene(sceneType::MENU);
                break;
        }
    }
    
    // Mouse interaction
    Vector2 mousePos = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    
    for (int i = 0; i < buttons.size(); i++)
    {
        Rectangle buttonRect = {
            buttons[i]->rect.x,
            buttons[i]->rect.y,
            buttons[i]->rect.width,
            buttons[i]->rect.height
        };
        
        if (CheckCollisionPointRec(mousePos, buttonRect))
        {
            if (selectedButton != i)
            {
                selectedButton = i;
            }
            
            if (mouseClicked)
            {
                switch (i)
                {
                    case 0: // Single Player Mario
                        GameInfo::getInstance()->setMode(Mode::SINGLE);
                        GameInfo::getInstance()->setCharacter(true);
                        // Don't start game immediately, just set the mode
                        break;
                    case 1: // Single Player Luigi
                        GameInfo::getInstance()->setMode(Mode::SINGLE);
                        GameInfo::getInstance()->setCharacter(false);
                        // Don't start game immediately, just set the mode
                        break;
                    case 2: // Two Player
                        GameInfo::getInstance()->setMode(Mode::DUAL);
                        // Don't start game immediately, just set the mode
                        break;
                    case 3: // Difficulty Easy
                        GameInfo::getInstance()->setDifficulty(Difficulty::EASY);
                        break;
                    case 4: // Difficulty Hard
                        GameInfo::getInstance()->setDifficulty(Difficulty::HARD);
                        break;
                    case 5: // Difficulty Hardcore
                        GameInfo::getInstance()->setDifficulty(Difficulty::HARDCORE);
                        break;
                    case 6: // Play - Start the game
                        manager->changeScene(sceneType::CHOOSE_LEVEL);
                        break;
                    case 7: // Back
                        manager->changeScene(sceneType::MENU);
                        break;
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
    
    // Logo pulsing animation
    logoScale += logoScaleDirection * GetFrameTime() * 0.5f;
    if (logoScale >= 0.35)
    {
        logoScale = 0.35f;
        logoScaleDirection = -1.0f;
    }
    else if (logoScale <= 0.25f)
    {
        logoScale = 0.25f;
        logoScaleDirection = 1.0f;
    }
}

void PreGame::updateScene()
{
    handleInput();
    updateAnimations();
    
    // Update button states
    for (int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->isHovered = (i == selectedButton);
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
    
    // Draw logo
    if (logoTexture.id > 0)
    {
        // Scale so it never exceeds 30% of screen width
        float maxW = UI::screenWidth * 0.25f;
        float scale = maxW / logoTexture.width;
        float drawW = logoTexture.width * scale;
        float drawH = logoTexture.height * scale;
        // apply your bounce offset to Y
        float drawX = (UI::screenWidth - drawW) / 2.0f;
        float drawY = 80 + sinf(logoBounce) * 10.0f;
        DrawTexturePro(
            logoTexture,
            {0, 0, (float)logoTexture.width, (float)logoTexture.height},
            {drawX, drawY, drawW, drawH},
            {0, 0}, 0.0f, WHITE);
    }
    
    // Draw title text
    std::string titleText = "SELECT GAME MODE & DIFFICULTY";
    float titleFontSize = 32.0f;
    float titleSpacing = 3.0f;
    Vector2 titleSize = MeasureTextEx(UI::boldFont, titleText.c_str(), titleFontSize, titleSpacing);
    float titleX = (UI::screenWidth - titleSize.x) / 2.0f;
    float titleY = UI::screenHeight / 2.0f - 50;
    
    // Title shadow
    DrawTextEx(UI::boldFont, titleText.c_str(),
               {titleX + 2, titleY + 2}, titleFontSize, titleSpacing, Color{0, 0, 0, 150});
    // Title text
    DrawTextEx(UI::boldFont, titleText.c_str(),
               {titleX, titleY}, titleFontSize, titleSpacing, Color{255, 255, 255, 255});
    
    // Draw buttons
    GameInfo* gameInfo = GameInfo::getInstance();
    
    for (int i = 0; i < buttons.size(); i++)
    {
        // Check if this button represents the current selection
        bool isChosen = false;
        
        // Check if this button represents the current game mode
        if (i == 0 && gameInfo->getMode() == Mode::SINGLE && gameInfo->isMario()) {
            isChosen = true; // Single Player Mario
        } 
        if (i == 1 && gameInfo->getMode() == Mode::SINGLE && !gameInfo->isMario()) {
            isChosen = true; // Single Player Luigi
        } 
        if (i == 2 && gameInfo->getMode() == Mode::DUAL) {
            isChosen = true; // Two Player
        }
        
        // Check if this button represents the current difficulty (separate checks)
        if (i == 3 && gameInfo->getDifficulty() == Difficulty::EASY) {
            isChosen = true; // Easy difficulty
        } 
        if (i == 4 && gameInfo->getDifficulty() == Difficulty::HARD) {
            isChosen = true; // Hard difficulty
        } 
        if (i == 5 && gameInfo->getDifficulty() == Difficulty::HARDCORE) {
            isChosen = true; // Hardcore difficulty
        }
        
        // Set button colors based on state
        Color fillColor;
        if (isChosen) {
            fillColor = Color{255, 165, 0, 255}; // Orange for chosen options
        } else {
            fillColor = Color{70, 130, 180, 255}; // Steel blue for default
        }
        
        // Override the button's fill color temporarily
        Color originalFillColor = buttons[i]->FillColor;
        buttons[i]->FillColor = fillColor;
        
        buttons[i]->draw();
        
        // Restore original color
        buttons[i]->FillColor = originalFillColor;
        
        // Add selection highlight for currently navigated button (yellow border)
        if (i == selectedButton)
        {
            Rectangle buttonRect = {
                buttons[i]->rect.x - 5,
                buttons[i]->rect.y - 5,
                buttons[i]->rect.width + 10,
                buttons[i]->rect.height + 10
            };
            DrawRectangleRoundedLines(buttonRect, 0.1f, 8, Color{255, 255, 0, 200});
        }
    }
    
    // Draw instructions
    std::string instruction = "Use ARROW KEYS or WASD to navigate (UP/DOWN between rows, LEFT/RIGHT within rows)";
    std::string instruction2 = "ENTER to select, ESC to go back";
    std::string instruction3 = "Select mode & difficulty, then press PLAY to start!";
    float instructionFontSize = 14.0f;
    Vector2 instructionSize = MeasureTextEx(UI::font, instruction.c_str(), instructionFontSize, 1);
    Vector2 instructionSize2 = MeasureTextEx(UI::font, instruction2.c_str(), instructionFontSize, 1);
    Vector2 instructionSize3 = MeasureTextEx(UI::font, instruction3.c_str(), instructionFontSize, 1);
    float instructionX = (UI::screenWidth - instructionSize.x) / 2.0f;
    float instructionX2 = (UI::screenWidth - instructionSize2.x) / 2.0f;
    float instructionX3 = (UI::screenWidth - instructionSize3.x) / 2.0f;
    float instructionY = UI::screenHeight - 90;
    float instructionY2 = UI::screenHeight - 70;
    float instructionY3 = UI::screenHeight - 50;
    
    DrawTextEx(UI::font, instruction.c_str(),
               {instructionX + 1, instructionY + 1}, instructionFontSize, 1, Color{0, 0, 0, 150});
    DrawTextEx(UI::font, instruction.c_str(),
               {instructionX, instructionY}, instructionFontSize, 1, Color{200, 200, 200, 255});
               
    DrawTextEx(UI::font, instruction2.c_str(),
               {instructionX2 + 1, instructionY2 + 1}, instructionFontSize, 1, Color{0, 0, 0, 150});
    DrawTextEx(UI::font, instruction2.c_str(),
               {instructionX2, instructionY2}, instructionFontSize, 1, Color{200, 200, 200, 255});
               
    DrawTextEx(UI::font, instruction3.c_str(),
               {instructionX3 + 1, instructionY3 + 1}, instructionFontSize, 1, Color{0, 0, 0, 150});
    DrawTextEx(UI::font, instruction3.c_str(),
               {instructionX3, instructionY3}, instructionFontSize, 1, Color{0, 255, 0, 255});
}