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
    buttonTexture = LoadTexture("assets/Backgrounds/Buttons/MenuButton.png");
    buttonHoverTexture = LoadTexture("assets/Backgrounds/Buttons/MenuButtonHovered.png");

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
    if (buttonHoverTexture.id!=0)
    {
        UnloadTexture(buttonHoverTexture);
        buttonHoverTexture.id = 0;
    }
    if (buttonTexture.id!=0)
    {
        UnloadTexture(buttonTexture);
        buttonTexture.id = 0;
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
            manager->curMap="Map1";
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
    float panelWidth = 350;
    float panelHeight = 400;
    float panelX = UI::screenWidth / 2 - panelWidth / 2;
    float panelY = UI::screenHeight / 2 - panelHeight / 2;
    float buttonWidth = 200;
    float buttonHeight = 50;
    float buttonX = panelX + (panelWidth - buttonWidth) / 2;
    float startY = panelY + 400;
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
                manager->curMap="Map1";
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
    std::string titleText = "GAME OVER";
    std::vector<std::string> buttonTexts = {
        "PLAY AGAIN",
        "BACK TO MENU"};

    // Draw semi-transparent overlay over the entire screen
    DrawRectangle(0, 0, UI::screenWidth, UI::screenHeight, Color{0, 0, 0, 30});

    // Calculate game over panel dimensions and position
    float panelWidth = 350;
    float panelHeight = 400;
    float panelX = UI::screenWidth / 2 - panelWidth / 2;
    float panelY = UI::screenHeight / 2 - panelHeight / 2;
    Rectangle panelRect = {panelX, panelY, panelWidth, panelHeight};

    // Draw title
    float titleSpacing = 3.0f;
    float titleFontSize = 36.0f;
    Vector2 titleSize = MeasureTextEx(UI::boldFont, titleText.c_str(), titleFontSize, titleSpacing);
    float titleX = (UI::screenWidth - titleSize.x) / 2.0f;
    float titleY = panelY + 20;

    // Title with shadow (red color for game over)
    DrawTextEx(UI::boldFont, titleText.c_str(),
               {titleX + 2, titleY + 2}, titleFontSize, titleSpacing, BLACK);
    DrawTextEx(UI::boldFont, titleText.c_str(),
               {titleX, titleY}, titleFontSize, titleSpacing, WHITE);

    // Draw final score and high score
    std::string scoreText = "Final Score: " + std::to_string(GameInfo::getInstance()->getScore());
    Vector2 scoreSize = MeasureTextEx(UI::font, scoreText.c_str(), 24, 2);
    std::string highScoreText = "High Score: " + std::to_string(GameInfo::getInstance()->getHighScore());
    Vector2 highScoreSize = MeasureTextEx(UI::font, highScoreText.c_str(), 24, 2);

    float scoreX = (UI::screenWidth - scoreSize.x) / 2.0f;
    float highScoreX = (UI::screenWidth - highScoreSize.x) / 2.0f;  
    float statsY = panelY + 120;

    // Score text with shadow
    DrawTextEx(UI::font, scoreText.c_str(), {scoreX + 1, statsY + 1}, 24, 2, BLACK);
    DrawTextEx(UI::font, scoreText.c_str(), {scoreX, statsY}, 24, 2, WHITE);

    DrawTextEx(UI::font, highScoreText.c_str(), {highScoreX + 1, statsY + 30 + 1}, 24, 2, BLACK);
    DrawTextEx(UI::font, highScoreText.c_str(), {highScoreX, statsY + 30}, 24, 2, WHITE);

    // Draw buttons
    float buttonWidth = 200;
    float buttonHeight = 50;
    float buttonX = panelX + (panelWidth - buttonWidth) / 2;
    float startY = panelY + 400;
    float spacing = 70;

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
        Vector2 textSize = MeasureTextEx(UI::font, buttonTexts[i].c_str(), 20, buttonTextSpacing);
        float textX = buttonRect.x + (buttonRect.width - textSize.x) / 2;
        float textY = buttonRect.y + (buttonRect.height - textSize.y) / 2;

        // Text with shadow
        DrawTextEx(UI::font, buttonTexts[i].c_str(),
                   {textX + 1, textY + 1}, 20, buttonTextSpacing, BLACK);
        DrawTextEx(UI::font, buttonTexts[i].c_str(),
                   {textX, textY}, 20, buttonTextSpacing, WHITE);

        // Flash effect for selected button
        if (flashActive && i == selectedButton)
        {
            float hue = fmodf(flashTimer * 360.0f / flashDuration, 360.0f);
            Color col = ColorFromHSV(hue, 1.0f, 1.0f);
            DrawTextEx(UI::font, buttonTexts[i].c_str(), {textX, textY}, 20, 2, col);
        }
    }

}
