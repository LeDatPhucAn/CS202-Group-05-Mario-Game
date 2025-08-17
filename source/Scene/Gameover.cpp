#include "GameOver.hpp"
#include "raylib.h"
#include <vector>
#include <string>
#include "GameInfo.hpp"

// Static variables for flash effect
static bool flashActive = false;
static float flashTimer = 0.0f;
static const float flashDuration = 0.6f;

GameOver::GameOver(SceneManager *_manager) : Scene(_manager)
{

    // Load button textures
    backgroundTexture = LoadTexture("assets/Backgrounds/GameOverBackground.png");
    buttonTexture = LoadTexture("assets/Backgrounds/Buttons/MenuButton.png");
    buttonHoverTexture = LoadTexture("assets/Backgrounds/Buttons/MenuButtonHovered.png");

    if (backgroundTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load background texture");
    }
    if (buttonTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load button texture");
    }
    if (buttonHoverTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load button hover texture");
    }

    selectedButton = 0;
    GameInfo::getInstance()->loadHighScoreFromFile();
    GameInfo::getInstance()->updateHighScore();
    SoundController::getInstance().playSceneMusicFromStart(sceneType::GAMEOVER);
}

GameOver::~GameOver()
{
    if (buttonHoverTexture.id != 0)
    {
        UnloadTexture(buttonHoverTexture);
        buttonHoverTexture.id = 0;
    }
    if (buttonTexture.id != 0)
    {
        UnloadTexture(buttonTexture);
        buttonTexture.id = 0;
    }
    if (backgroundTexture.id != 0)
    {
        UnloadTexture(backgroundTexture);
        backgroundTexture.id = 0;
    }
}

void GameOver::updateScene()
{
    // Handle flash effect
    if (flashActive)
    {
        flashTimer += GetFrameTime();
        if (flashTimer >= flashDuration)
        {
            flashActive = false;
            flashTimer = 0.0f;
        }
        return;
    }

    // Navigation
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        selectedButton = (selectedButton - 1 + 2) % 2;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        selectedButton = (selectedButton + 1) % 2;
    }

    // Activate button
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        switch (selectedButton)
        {
        case 0: // Play Again
            GameInfo::getInstance()->reset();
            SoundController::getInstance().playSceneMusicFromStart(sceneType::GAME);
            manager->goBack();
            manager->curMap = "Map1";
            return;
        case 1: // Back to Menu
            GameInfo::getInstance()->reset();
            SoundController::getInstance().playSceneMusicFromStart(sceneType::MENU);
            manager->goBackOfBaseScene();
            return;
        }
    }

    // ESC to go back to menu
    if (IsKeyPressed(KEY_P))
    {
        GameInfo::getInstance()->reset();
        manager->goBackOfBaseScene();
        flashActive = true;
        flashTimer = 0.0f;
        return;
    }

    // Mouse interaction
    Vector2 mousePos = GetMousePosition();
    // Panel variables removed; derive layout directly from screen center
    float centerY = UI::screenHeight / 2.0f;
    float buttonWidth = 200;
    float buttonHeight = 50;
    // Left aligned layout
    float leftMargin = 60.0f;
    float buttonX = leftMargin + 70.0f;

    float startY = centerY;
    float spacing = 70;

    for (int i = 0; i < 2; ++i)
    {
        Rectangle btnRect = {
            (float)buttonX,
            (float)(startY + i * spacing),
            (float)buttonWidth,
            (float)buttonHeight};
        if (CheckCollisionPointRec(mousePos, btnRect))
        {
            selectedButton = i;
            break;
        }
    }

    // Handle mouse clicks
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Rectangle btnRect = {
            (float)buttonX,
            (float)(startY + selectedButton * spacing),
            (float)buttonWidth,
            (float)buttonHeight};
        if (CheckCollisionPointRec(mousePos, btnRect))
        {
            switch (selectedButton)
            {
            case 0: // Play Again
                GameInfo::getInstance()->reset();
                SoundController::getInstance().playSceneMusicFromStart(sceneType::GAME);
                manager->curMap = "Map1";
                manager->shouldReloadGame = true;
                manager->goBack();
                return;
            case 1: // Back to Menu
                GameInfo::getInstance()->reset();
                SoundController::getInstance().playSceneMusicFromStart(sceneType::MENU);
                manager->goBackOfBaseScene();
                return;
            }
        }
    }
}

void GameOver::displayScene()
{

    // Load background textures
    if (backgroundTexture.id > 0)
    {
        DrawTexturePro(backgroundTexture,
                       {0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
                       {0, 0, (float)UI::screenWidth, (float)UI::screenHeight},
                       {0, 0}, 0.0f, WHITE);
    }
    else
    {
        ClearBackground(BLACK);
    }

    std::string titleText = "GAME OVER";
    std::vector<std::string> buttonTexts = {
        "PLAY AGAIN",
        "BACK TO MENU"};

    // Draw semi-transparent overlay over the entire screen
    DrawRectangle(0, 0, UI::screenWidth, UI::screenHeight, Color{0, 0, 0, 30});

    // Calculate game over panel dimensions and position
    // Panel removed; compute positions relative to screen center
    float centerY = UI::screenHeight / 2.0f;
    float leftMargin = 60.0f; // uniform left margin for left-aligned text

    // Text metrics
    float titleSpacing = 3.0f;
    float titleFontSize = 60.0f;
    Vector2 titleSize = MeasureTextEx(UI::boldFont, titleText.c_str(), titleFontSize, titleSpacing);
    float titleX = leftMargin;
    float titleY = centerY - 240.0f;

    std::string scoreText = "Final Score: " + std::to_string(GameInfo::getInstance()->getScore());
    Vector2 scoreSize = MeasureTextEx(UI::font, scoreText.c_str(), 36, 2);
    std::string highScoreText = "High Score: " + std::to_string(GameInfo::getInstance()->getHighScore());
    Vector2 highScoreSize = MeasureTextEx(UI::font, highScoreText.c_str(), 36, 2);

    float scoreX = leftMargin + 35.0f;
    float highScoreX = leftMargin + 35.0f;
    float statsY = centerY - 150.0f;

    // Button layout (defined early so backdrop can include them)
    const int buttonCount = 2;
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float buttonX = leftMargin + 70.0f;
    float spacing = 70.0f;
    float startY = centerY; // top of first button
    float buttonsBottom = startY + (buttonCount - 1) * spacing + buttonHeight;

    // Compute a single transparent background rectangle behind the text + buttons
    float padX = 20.0f;
    float padYTop = 15.0f;
    float padYBottom = 25.0f;
    float blockLeft = leftMargin - padX;
    float blockTop = titleY - padYTop;
    float blockBottom = buttonsBottom + padYBottom; // extend through buttons
    float blockHeight = blockBottom - blockTop;
    // Determine max width among title, score, high score, and button area
    float maxWidth = titleSize.x;
    if (scoreSize.x > maxWidth)
        maxWidth = scoreSize.x;
    if (highScoreSize.x > maxWidth)
        maxWidth = highScoreSize.x;
    if (buttonWidth > maxWidth)
        maxWidth = buttonWidth;
    float blockWidth = maxWidth + padX * 2.0f;

    // Draw translucent backdrop (black with alpha)
    DrawRectangleRounded({blockLeft, blockTop, blockWidth, blockHeight}, 0.08f, 6, Color{0, 0, 0, 120});
    DrawRectangleRoundedLines({blockLeft, blockTop, blockWidth, blockHeight}, 0.08f, 6, Color{255, 255, 255, 40});

    // Title with shadow
    DrawTextEx(UI::boldFont, titleText.c_str(), {titleX + 2, titleY + 2}, titleFontSize, titleSpacing, BLACK);
    DrawTextEx(UI::boldFont, titleText.c_str(), {titleX + 1, titleY + 2}, titleFontSize, titleSpacing, BLACK);
    DrawTextEx(UI::boldFont, titleText.c_str(), {titleX + 2, titleY + 1}, titleFontSize, titleSpacing, BLACK);
    DrawTextEx(UI::boldFont, titleText.c_str(), {titleX + 2, titleY + 1}, titleFontSize, titleSpacing, BLACK);
    DrawTextEx(UI::boldFont, titleText.c_str(), {titleX, titleY}, titleFontSize, titleSpacing, WHITE);

    // Score text with shadow
    DrawTextEx(UI::font, scoreText.c_str(), {scoreX + 1, statsY + 1}, 36, 2, BLACK);
    DrawTextEx(UI::font, scoreText.c_str(), {scoreX + 1, statsY + 2}, 36, 2, BLACK);
    DrawTextEx(UI::font, scoreText.c_str(), {scoreX + 2, statsY + 1}, 36, 2, BLACK);
    DrawTextEx(UI::font, scoreText.c_str(), {scoreX + 2, statsY + 2}, 36, 2, BLACK);
    DrawTextEx(UI::font, scoreText.c_str(), {scoreX, statsY}, 36, 2, WHITE);
    DrawTextEx(UI::font, highScoreText.c_str(), {highScoreX + 1, statsY + 30 + 1}, 36, 2, BLACK);
    DrawTextEx(UI::font, highScoreText.c_str(), {highScoreX + 2, statsY + 30 + 1}, 36, 2, BLACK);
    DrawTextEx(UI::font, highScoreText.c_str(), {highScoreX + 1, statsY + 30 + 2}, 36, 2, BLACK);
    DrawTextEx(UI::font, highScoreText.c_str(), {highScoreX + 2, statsY + 30 + 2}, 36, 2, BLACK);
    DrawTextEx(UI::font, highScoreText.c_str(), {highScoreX, statsY + 30}, 36, 2, WHITE);

    // Draw buttons (variables already declared above)

    for (int i = 0; i < 2; i++)
    {
        Rectangle buttonRect = {buttonX, startY + i * spacing, buttonWidth, buttonHeight};

        // Draw button texture background
        if (buttonTexture.id > 0)
        {
            // Use hover texture for selected button, normal texture for others
            Texture2D currentTexture = (i == selectedButton) ? buttonHoverTexture : buttonTexture;

            // Check if hover texture is available, fallback to normal texture
            if (i == selectedButton && buttonHoverTexture.id == 0)
            {
                currentTexture = buttonTexture;
            }

            DrawTexturePro(currentTexture,
                           {0, 0, (float)currentTexture.width, (float)currentTexture.height},
                           buttonRect,
                           {0, 0}, 0.0f, WHITE);

            // Add selection highlight border if selected
            if (i == selectedButton)
            {
                DrawRectangleRoundedLines(buttonRect, 0.2f, 10, ORANGE);
            }
        }
        else
        {
            // Fallback to solid color buttons if textures fail to load
            Color buttonColor = (i == selectedButton) ? Color{160, 82, 45, 255} : Color{101, 67, 33, 255};
            DrawRectangleRounded(buttonRect, 0.2f, 10, buttonColor);

            // Draw selection highlight
            if (i == selectedButton)
            {
                DrawRectangleRoundedLines(buttonRect, 0.2f, 10, ORANGE);
            }
            else
            {
                DrawRectangleRoundedLines(buttonRect, 0.2f, 10, Color{139, 69, 19, 255});
            }
        }

        // Button text (centered)
        float buttonTextSpacing = 2.0f;
        Vector2 textSize = MeasureTextEx(UI::font, buttonTexts[i].c_str(), 24, buttonTextSpacing);
        float textX = buttonRect.x + 30.0f; // left aligned within button with padding
        float textY = buttonRect.y + (buttonRect.height - textSize.y) / 2;

        // Text with shadow
        DrawTextEx(UI::font, buttonTexts[i].c_str(),
                   {textX + 1, textY + 1}, 24, buttonTextSpacing, BLACK);
        DrawTextEx(UI::font, buttonTexts[i].c_str(),
                   {textX + 1, textY + 2}, 24, buttonTextSpacing, BLACK);
        DrawTextEx(UI::font, buttonTexts[i].c_str(),
                   {textX + 2, textY + 1}, 24, buttonTextSpacing, BLACK);
        DrawTextEx(UI::font, buttonTexts[i].c_str(),
                   {textX + 2, textY + 2}, 24, buttonTextSpacing, BLACK);
        DrawTextEx(UI::font, buttonTexts[i].c_str(),
                   {textX, textY}, 24, buttonTextSpacing, WHITE);

        // Flash effect for selected button
        if (flashActive && i == selectedButton)
        {
            float hue = fmodf(flashTimer * 360.0f / flashDuration, 360.0f);
            Color col = ColorFromHSV(hue, 1.0f, 1.0f);
            DrawTextEx(UI::font, buttonTexts[i].c_str(), {textX, textY}, 20, 2, col);
        }
    }
}
