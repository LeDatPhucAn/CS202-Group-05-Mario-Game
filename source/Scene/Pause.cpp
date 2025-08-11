#include "Pause.hpp"
#include "raylib.h"
#include <vector>
#include <string>
#include "GameInfo.hpp"
// Static variables for flash effect
static bool flashActive = false;
static float flashTimer = 0.0f;
// static sceneType   nextScene      = sceneType::GAME;
static const float flashDuration = 0.6f;

Pause::Pause(SceneManager *_manager) : Scene(_manager)
{
    // Load pause overlay texture (optional small panel)
    pausePanel = LoadTexture("assets/Backgrounds/SettingsBoard.png");

    if (pausePanel.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load pause panel texture");
    }

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
    // play pause SFX
    SoundController::getInstance().playSceneSFX(sceneType::PAUSE);
    // Pause the music being played
    SoundController::getInstance().pauseSceneMusic();
    selectedButton = 0;
}

Pause::~Pause()
{
    // unpause the music
    SoundController::getInstance().resumeSceneMusic();
    if (pausePanel.id > 0)
    {
        UnloadTexture(pausePanel);
    }
}

void Pause::updateScene()
{
    // Handle flash effect
    // if (flashActive) {
    //     flashTimer += GetFrameTime();
    //     if (flashTimer >= flashDuration) {
    //         flashActive = false;
    //         flashTimer = 0.0f;
    //         manager->changeScene(nextScene);
    //     }
    //     return;
    // }

    // Navigation
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        selectedButton = (selectedButton - 1 + 4) % 4;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        selectedButton = (selectedButton + 1) % 4;
    }

    // Activate button
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        switch (selectedButton)
        {
        case 0: // Resume
            manager->goBack();
            return;
        case 1: // Save
                manager->changeScene(sceneType::TUTORIAL);
                return;
        case 2: // Settings
            manager->changeScene(sceneType::SETTING);
            return;
        case 3: // Exit
            GameInfo::getInstance()->reset();
            manager->goBackOfBaseScene();
            flashActive = true;
            flashTimer = 0.0f;
            return;
        }
    }

    // // ESC to resume game
    // if (IsKeyPressed(KEY_ESCAPE)) {
    //     nextScene = sceneType::GAME;
    //     flashActive = true;
    //     flashTimer = 0.0f;
    //     return;
    // }

    // Mouse interaction
    Vector2 mousePos = GetMousePosition();
    float panelWidth = 300;
    float panelHeight = 350;
    float panelX = UI::screenWidth / 2 - panelWidth / 2;
    float panelY = UI::screenHeight / 2 - panelHeight / 2;
    float buttonWidth = 200;
    float buttonHeight = 50;
    float buttonX = panelX + (panelWidth - buttonWidth) / 2;
    float startY = panelY + 80;
    float spacing = 60;

    for (int i = 0; i < 4; ++i)
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
            case 0: // Resume
            manager->goBack();
            return;
            case 1: // Controls
                manager->changeScene(sceneType::TUTORIAL);
                return;
            case 2: // Settings
                manager->changeScene(sceneType::SETTING);
                return;
            case 3: // Exit
                GameInfo::getInstance()->reset();
                manager->goBackOfBaseScene();
                flashActive = true;
                flashTimer = 0.0f;
                return;
            }
        }
    }
}

void Pause::displayScene()
{

    std::string titleText = "PAUSED";
    std::vector<std::string> buttonTexts = {
        "RESUME",
        "CONTROLS",
        "SETTINGS",
        "EXIT"};

    // Draw semi-transparent overlay over the entire screen
    DrawRectangle(0, 0, UI::screenWidth, UI::screenHeight, Color{0, 0, 0, 50});

    // Calculate pause panel dimensions and position
    float panelWidth = 300;
    float panelHeight = 350;
    float panelX = UI::screenWidth / 2 - panelWidth / 2;
    float panelY = UI::screenHeight / 2 - panelHeight / 2;
    Rectangle panelRect = {panelX, panelY, panelWidth, panelHeight};

    // Draw the pause panel
    if (pausePanel.id > 0)
    {
        DrawTexturePro(pausePanel,
                       {0, 0, (float)pausePanel.width, (float)pausePanel.height},
                       panelRect,
                       {0, 0}, 0.0f, WHITE);
    }
    else
    {
        // Fallback panel background
        DrawRectangleRounded(panelRect, 0.1f, 10, Color{139, 69, 19, 220});
        DrawRectangleRoundedLines(panelRect, 0.1f, 10, Color{101, 67, 33, 255});
    }

    // Draw title
    float titleSpacing = 3.0f;
    float titleFontSize = 36.0f;
    Vector2 titleSize = MeasureTextEx(UI::boldFont, titleText.c_str(), titleFontSize, titleSpacing);
    float titleX = (UI::screenWidth - titleSize.x) / 2.0f;
    float titleY = panelY + 20;

    // Title with shadow
    DrawTextEx(UI::boldFont, titleText.c_str(),
               {titleX + 2, titleY + 2}, titleFontSize, titleSpacing, BLACK);
    DrawTextEx(UI::boldFont, titleText.c_str(),
               {titleX, titleY}, titleFontSize, titleSpacing, WHITE);

    // Draw buttons
    float buttonWidth = 200;
    float buttonHeight = 50;
    float buttonX = panelX + (panelWidth - buttonWidth) / 2;
    float startY = panelY + 80;
    float spacing = 60;

    for (int i = 0; i < 4; i++)
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
        Vector2 textSize = MeasureTextEx(UI::font, buttonTexts[i].c_str(), 22, buttonTextSpacing);
        float textX = buttonRect.x + (buttonRect.width - textSize.x) / 2;
        float textY = buttonRect.y + (buttonRect.height - textSize.y) / 2;

        // Text with shadow
        DrawTextEx(UI::font, buttonTexts[i].c_str(),
                   {textX + 1, textY + 1}, 22, buttonTextSpacing, WHITE);
        DrawTextEx(UI::font, buttonTexts[i].c_str(),
                   {textX, textY}, 22, buttonTextSpacing, BLACK);

        // Flash effect for selected button
        if (flashActive && i == selectedButton)
        {
            float t = fmodf(flashTimer / flashDuration, 1.0f);
            float hue = fmodf(flashTimer * 360.0f / flashDuration, 360.0f);
            Color col = ColorFromHSV(hue, 1.0f, 1.0f);
            DrawTextEx(UI::font, buttonTexts[i].c_str(), {textX, textY}, 22, 2, col);
        }
    }

    // Draw instructions
    string instructions = "Use ARROW KEYS to navigate - ENTER to select - ESC to resume";
    Vector2 instrSize = MeasureTextEx(UI::font, instructions.c_str(), 16, 2);
    float instrX = (UI::screenWidth - instrSize.x) / 2;
    float instrY = panelY + panelHeight + 20;

    DrawTextEx(UI::font, instructions.c_str(),
               {instrX + 1, instrY + 1}, 20, 2, BLACK);
    DrawTextEx(UI::font, instructions.c_str(),
               {instrX, instrY}, 20, 2, WHITE);
}