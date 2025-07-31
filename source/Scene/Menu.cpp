#include "Menu.hpp"
#include "raylib.h"
#include <cmath>
#include <vector>
#include <string>

// Static variable for title animation
static float titleBounce = 0.0f;
static bool buttonsInitialized = false;

// Static variables for flash effect
static bool flashActive = false;
static float flashTimer = 0.0f;
static sceneType nextScene = sceneType::GAME;
static const float flashDuration = 0.6f;

Menu::Menu(SceneManager *_manager) : Scene(_manager)
{
    // Load the background texture
    backgroundTexture = LoadTexture("assets/Backgrounds/MenuBackground.png");

    if (backgroundTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load menu background texture");
    }

    // Load the title texture
    titleTexture = LoadTexture("assets/Backgrounds/Title.png");
    if (titleTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load title texture");
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

    // Load button textures into vector
    vector<std::string> iconPaths = {
        "assets/Backgrounds/Buttons/StartButtonIcon.png",
        "assets/Backgrounds/Buttons/ChooseLVButtonIcon.png",
        "assets/Backgrounds/Buttons/SettingButtonIcon.png",
        "assets/Backgrounds/Buttons/ExitButtonIcon.png"};

    for (const auto &path : iconPaths)
    {
        Texture2D texture = LoadTexture(path.c_str());
        if (texture.id == 0)
        {
            TraceLog(LOG_WARNING, "Failed to load button icon texture");
        }
        buttonTextures.push_back(texture);
    }

    selectionIconTexture = LoadTexture("assets/Backgrounds/Buttons/SelectionIcon.png");
    if (selectionIconTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load selection icon texture");
    }
}

Menu::~Menu()
{
    // Clean up texture
    if (backgroundTexture.id > 0)
    {
        UnloadTexture(backgroundTexture);
    }

    if (titleTexture.id > 0)
    {
        UnloadTexture(titleTexture);
    }
    if (buttonTexture.id > 0)
    {
        UnloadTexture(buttonTexture);
    }
    if (buttonHoverTexture.id > 0)
    {
        UnloadTexture(buttonHoverTexture);
    }

    // Clean up buttons
    for (auto button : buttons)
    {
        delete button;
        button = nullptr;
    }
    buttons.clear();

    // Clean up button textures
    for (auto &texture : buttonTextures)
    {
        if (texture.id > 0)
        {
            UnloadTexture(texture);
        }
    }
    buttonTextures.clear();

    if (selectionIconTexture.id > 0)
    {
        UnloadTexture(selectionIconTexture);
    }
}

void Menu::updateScene()
{

    // flash effect
    if (flashActive)
    {
        flashTimer += GetFrameTime();
        if (flashTimer >= flashDuration)
        {
            flashActive = false;
            flashTimer = 0.0f;
            manager->changeScene(nextScene);
        }
        return; // skip other updates if flash is active
    }

    // Initialize buttons once
    if (!buttonsInitialized)
    {
        std::vector<std::string> buttonTexts = {
            "START GAME",
            "CHOOSE LEVEL",
            "SETTINGS",
            "EXIT GAME"};

        // init buttons(color not important)
        for (int i = 0; i < buttonTexts.size(); i++)
        {
            buttons.push_back(new TextBox(buttonTexts[i], 0, 0, WHITE, ORANGE, BLACK));
        }

        buttonsInitialized = true;
    }

    titleBounce += GetFrameTime() * 2.0f;

    // Selection based on keyboard input
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        selectedButton = (selectedButton - 1 + 4) % 4;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        selectedButton = (selectedButton + 1) % 4;
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        switch (selectedButton)
        {
        case 0:
            nextScene = sceneType::GAME;
            break;
        case 1:
            nextScene = sceneType::CHOOSE_LEVEL;
            break;
        case 2:
            nextScene = sceneType::SETTING;
            break;
        case 3:
            CloseWindow();
            break;
        }
        flashActive = true;
        flashTimer = 0.0f;
        return;
    }

    // Selection based on mouse click
    Vector2 mousePos = GetMousePosition();
    int buttonWidth = 250;
    int buttonHeight = 60;
    int buttonX = UI::screenWidth / 2 - buttonWidth / 2;
    int startY = UI::screenHeight / 2 - 100;
    int spacing = 80;

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
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {

        for (int i = 0; i < 4; i++)
        {
            Rectangle buttonRect = {buttonX, startY + i * spacing, buttonWidth, buttonHeight};
            if (CheckCollisionPointRec(mousePos, buttonRect))
            {
                selectedButton = i;
                // Execute button action
                switch (i)
                {
                case 0:
                    nextScene = sceneType::GAME;
                    break;
                case 1:
                    nextScene = sceneType::CHOOSE_LEVEL;
                    break;
                case 2:
                    nextScene = sceneType::SETTING;
                    break;
                case 3:
                    CloseWindow();
                    break;
                }
                flashActive = true;
                flashTimer = 0.0f;
                return;
            }
        }
    }

    if (IsKeyPressed(KEY_N))
    {
        manager->changeScene(sceneType::GAME);
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        CloseWindow();
    }

    // Update buttons for mouse interaction
    for (auto button : buttons)
    {
        button->update();
    }
}

void Menu::displayScene()
{
    // Texts
    std::string titleText = "SUPER MARIO BROS";
    std::string subtitleText = "CS202 Group 05";
    std::string instructionsText = "Use ARROW KEYS/W-S to navigate \n ENTER/SPACE to select \n ESC to exit \n N for quick start";

    std::vector<std::string> buttonTexts = {
        "START GAME",
        "CHOOSE LEVEL",
        "SETTINGS",
        "EXIT GAME"};

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

    if (titleTexture.id > 0)
    {
        // Scale so it never exceeds 30% of screen width
        float maxW = UI::screenWidth * 0.25f;
        float scale = maxW / titleTexture.width;
        float drawW = titleTexture.width * scale;
        float drawH = titleTexture.height * scale;
        // apply your bounce offset to Y
        float drawX = (UI::screenWidth - drawW) / 2.0f;
        float drawY = 80 + sinf(titleBounce) * 10.0f;
        DrawTexturePro(
            titleTexture,
            {0, 0, (float)titleTexture.width, (float)titleTexture.height},
            {drawX, drawY, drawW, drawH},
            {0, 0}, 0.0f, WHITE);
    }
    else
    {
        // fallback to original text‐drawing code
        float titleSpacing = 12.0f;
        float titleFontSize = 72.0f;
        float titleX = (UI::screenWidth - 750) / 2.0f;
        float titleY = 80 + sinf(titleBounce) * 10.0f;

        // Draw title with outline effect
        DrawTextEx(UI::boldFont, titleText.c_str(),
                   {titleX + 2, titleY + 2}, titleFontSize, titleSpacing, ORANGE);
        DrawTextEx(UI::boldFont, titleText.c_str(),
                   {titleX, titleY}, titleFontSize, titleSpacing, RED);
    }
    // Subtitle centering
    float subtitleSpacing = 3.0f;
    float subtitleFontSize = 24.0f;

    float subtitleX = (UI::screenWidth) / 2.0f;
    float subtitleY = 275 + sinf(titleBounce) * 10.0f;

    // Draw title with outline effect
    // Draw subtitle with shadow
    DrawTextEx(UI::boldFont, subtitleText.c_str(),
               {subtitleX + 1, subtitleY + 1}, subtitleFontSize, subtitleSpacing, ORANGE);
    DrawTextEx(UI::boldFont, subtitleText.c_str(),
               {subtitleX, subtitleY}, subtitleFontSize, subtitleSpacing, RED);

    // Position and draw custom textured buttons
    float buttonWidth = 250;
    float buttonHeight = 60;
    float buttonX = UI::screenWidth / 2 - buttonWidth / 2;
    float startY = UI::screenHeight / 2 - 100;
    float spacing = 80;

    for (int i = 0; i < 4; i++)
    {
        Rectangle buttonRect = {buttonX, startY + i * spacing, buttonWidth, buttonHeight};

        // Choose texture based on selection
        Texture2D currentTexture = (i == selectedButton) ? buttonHoverTexture : buttonTexture;

        // Draw button texture
        if (currentTexture.id > 0)
        {
            DrawTexturePro(currentTexture,
                           {0, 0, (float)currentTexture.width, (float)currentTexture.height},
                           buttonRect,
                           {0, 0}, 0.0f, WHITE);
        }
        else
        {
            // Draw an orange rectangle if texture fails to load
            Color buttonColor = (i == selectedButton) ? ORANGE : Color{255, 165, 0, 255};
            DrawRectangleRounded(buttonRect, 0.2f, 10, buttonColor);
        }

        // Button text centering
        float buttonTextSpacing = 3.0f;
        Vector2 textSizeNoSpacing = MeasureTextEx(UI::font, buttonTexts[i].c_str(), 24, 0);
        float textX = buttonRect.x + (buttonRect.width - 200) / 2;
        float textY = buttonRect.y + (buttonRect.height - 24) / 2;

        // Text with shadow for better visibility
        DrawTextEx(UI::font, buttonTexts[i].c_str(),
                   {textX + 2, textY + 2}, 20, buttonTextSpacing, BLACK);
        DrawTextEx(UI::font, buttonTexts[i].c_str(),
                   {textX, textY}, 20, buttonTextSpacing, WHITE);

        // Flash effect
        if (flashActive && i == selectedButton)
        {
            float t = fmodf(flashTimer / flashDuration, 1.0f);
            float hue = fmodf(flashTimer * 360.0f / flashDuration, 360.0f);
            Color col = ColorFromHSV(hue, 1.0f, 1.0f);
            DrawTextEx(UI::font, buttonTexts[i].c_str(), {textX, textY}, 20, 3, col);
        }

        if (i < buttonTextures.size() && buttonTextures[i].id > 0)
        {
            float padding = 10.0f;
            float maxH = buttonRect.height - padding * 2;
            float scale = maxH / buttonTextures[i].height;
            float iconW = buttonTextures[i].width * scale;
            float iconH = buttonTextures[i].height * scale;
            float iconX = buttonRect.x + buttonRect.width - iconW - padding;
            float iconY = buttonRect.y + (buttonRect.height - iconH) * 0.5f;

            // Draw scaled icon
            DrawTexturePro(
                buttonTextures[i],
                {0.0f, 0.0f, (float)buttonTextures[i].width, (float)buttonTextures[i].height}, // source
                {iconX, iconY, iconW, iconH},                                                  // dest
                {0.0f, 0.0f}, 0.0f, WHITE);
        }
    }

    // Draw selection icon
    if (selectionIconTexture.id > 0)
    {
        float iconScale = 0.15f;
        float iconW = selectionIconTexture.width * iconScale;
        float iconH = selectionIconTexture.height * iconScale;
        float iconX = buttonX - iconW - 10;
        float iconY = startY + selectedButton * spacing + (buttonHeight - iconH) * 0.5f;

        DrawTexturePro(
            selectionIconTexture,
            {0, 0,
             (float)selectionIconTexture.width,
             (float)selectionIconTexture.height},
            {iconX, iconY,
             iconW, iconH},
            {0, 0}, 0.0f, WHITE);
    }
    else
    {
        // fallback to yellow triangle
        float triY = startY + selectedButton * spacing + 30;
        DrawTriangle(
            {buttonX - 30, triY - 10},
            {buttonX - 30, triY + 10},
            {buttonX - 15, triY},
            YELLOW);
    }

    float instrSpacing = 2.0f;
    float instrX = UI::screenWidth - 300;
    float instrY = UI::screenHeight - 200;

    DrawTextEx(UI::font, instructionsText.c_str(),
               {instrX + 1, instrY + 1}, 16, instrSpacing, DARKGRAY);
    DrawTextEx(UI::font, instructionsText.c_str(),
               {instrX, instrY}, 16, instrSpacing, WHITE);
}