#include "WinLevel.hpp"
#include "Menu.hpp"
#include "Scene.hpp"
#include "Game.hpp"
#include "GameInfo.hpp"
#include <raylib.h>

WinLevel::WinLevel(SceneManager *manager)
    : Scene(manager), anyKeyPressed(false), frameCount(0), currentFrame(0),
      frameTime(0.0f), frameDuration(0.03f), gifLoaded(false),
      victoryFrameCount(0), victoryCurrentFrame(0), victoryFrameTime(0.0f), victoryGifLoaded(false)
{
    mapName = manager->curMap;
    SoundController::getInstance().playSceneSFX(sceneType::WIN_LEVEL);
    SoundController::getInstance().playSceneMusicFromStart(sceneType::CREDITS);

    backgroundTexture = LoadTexture("assets/Backgrounds/WinLevelBackground.png");
    if (backgroundTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load WinLevel background texture");
    }
    
    // Load GIF animation for level completion (Map1 and Map2)
    gifImage = LoadImageAnim("assets/Backgrounds/MarioWin.gif", &frameCount);
    if (gifImage.data != nullptr && frameCount > 0)
    {
        gifTexture = LoadTextureFromImage(gifImage);
        gifLoaded = true;
    }
    else
    {
        TraceLog(LOG_WARNING, "Failed to load MarioWin.gif");
        gifLoaded = false;
    }

    // Load victory GIF animation for final victory (Map3)
    victoryGifImage = LoadImageAnim("assets/Backgrounds/MarioVictory.gif", &victoryFrameCount);
    if (victoryGifImage.data != nullptr && victoryFrameCount > 0)
    {
        victoryGifTexture = LoadTextureFromImage(victoryGifImage);
        victoryGifLoaded = true;
    }
    else
    {
        TraceLog(LOG_WARNING, "Failed to load MarioVictory.gif");
        victoryGifLoaded = false;
    }
}

WinLevel::~WinLevel()
{
    if (gifLoaded)
    {
        UnloadTexture(gifTexture);
        UnloadImage(gifImage);
    }
    if (victoryGifLoaded)
    {
        UnloadTexture(victoryGifTexture);
        UnloadImage(victoryGifImage);
    }
    if (backgroundTexture.id != 0)
    {
        UnloadTexture(backgroundTexture);
    }
}

void WinLevel::updateScene()
{
    float dt = GetFrameTime();

    // Update level completion GIF (Map1 and Map2)
    if (gifLoaded && frameCount > 1 && (mapName == "Map1" || mapName == "Map2"))
    {
        frameTime += dt;
        if (frameTime >= frameDuration)
        {
            frameTime -= frameDuration;
            currentFrame = (currentFrame + 1) % frameCount;
            int pixelsPerFrame = gifImage.width * gifImage.height * 4; // RGBA32
            unsigned char *framePtr = ((unsigned char *)gifImage.data) + (currentFrame * pixelsPerFrame);
            UpdateTexture(gifTexture, framePtr);
        }
    }

    // Update victory GIF (Map3)
    if (victoryGifLoaded && victoryFrameCount > 1 && mapName == "Map3")
    {
        victoryFrameTime += dt;
        if (victoryFrameTime >= 0.1f)
        {
            victoryFrameTime -= 0.1f;
            victoryCurrentFrame = (victoryCurrentFrame + 1) % victoryFrameCount;
            int pixelsPerFrame = victoryGifImage.width * victoryGifImage.height * 4; // RGBA32
            unsigned char *framePtr = ((unsigned char *)victoryGifImage.data) + (victoryCurrentFrame * pixelsPerFrame);
            UpdateTexture(victoryGifTexture, framePtr);
        }
    }

    if (!anyKeyPressed && GetKeyPressed() != 0)
    {
        anyKeyPressed = true;
        if (mapName == "Map1" || mapName == "Map2")
        {
            // Move to next level
            if (manager->curMap == "Map1")
                manager->curMap = "Map2";
            else if (manager->curMap == "Map2")
                manager->curMap = "Map3";
            manager->shouldReloadGame = true;
            manager->goBack();
        }
        else if (mapName == "Map3")
        {
            manager->goBackOfBaseScene(); // This should go to main menu
        }
    }
}

void WinLevel::displayScene()
{
    ClearBackground(BLACK);
    
    if (backgroundTexture.id > 0)
    {
        float scale = 1.0f;
        Rectangle source = {0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height};
        Rectangle dest = {0, 0, (float)backgroundTexture.width * scale, (float)backgroundTexture.height * scale};
        DrawTexturePro(backgroundTexture, source, dest, {0, 0}, 0.0f, WHITE);
    }

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int fontSize = 120;
    int subFontSize = 40;
    int promptFontSize = 24;

    const char *title = "";
    const char *subtitle = "";
    const char *prompt = "";
    static string titleStr = "";
    string score;
    string highScore;
    if (mapName == "Map1" || mapName == "Map2")
    {
        titleStr = (mapName == "Map1") ? "Level 1 Completed" : "Level 2 Completed";
        title = titleStr.c_str();
        subtitle = "You almost save the princess, keep going";
        prompt = "Press any key to move to next level";

        if (gifLoaded)
        {
            float scale = 1.0f; // Make Mario 80% smaller
            int scaledWidth = (int)(gifTexture.width * scale);
            int scaledHeight = (int)(gifTexture.height * scale);
            int gifX = screenWidth / 2 - scaledWidth / 2;
            int gifY = screenHeight / 2 - 100;

            Rectangle source = {0, 0, (float)gifTexture.width, (float)gifTexture.height};
            Rectangle dest = {(float)gifX, (float)gifY, (float)scaledWidth, (float)scaledHeight};
            Vector2 origin = {0, 0};

            DrawTexturePro(gifTexture, source, dest, origin, 0.0f, WHITE);
        }
    }
    else if (mapName == "Map3")
    {
        title = "Victory";
        subtitle = "You have saved the princess";
        score = "Score: " + to_string(GameInfo::getInstance()->getScore());
        highScore = "High Score: " + to_string(GameInfo::getInstance()->getHighScore());
        prompt = "Press any key to go back to the menu";

        if (victoryGifLoaded)
        {
            float scale = 1.2f; // Make Mario 100% size
            int scaledWidth = (int)(victoryGifTexture.width * scale);
            int scaledHeight = (int)(victoryGifTexture.height * scale);
            int gifX = screenWidth / 2 - scaledWidth / 2;
            int gifY = screenHeight / 2 - 60;

            Rectangle source = {0, 0, (float)victoryGifTexture.width, (float)victoryGifTexture.height};
            Rectangle dest = {(float)gifX, (float)gifY, (float)scaledWidth, (float)scaledHeight};
            Vector2 origin = {0, 0};

            DrawTexturePro(victoryGifTexture, source, dest, origin, 0.0f, WHITE);
        }
    }

    
    //draw shadow for title
    DrawText(title, screenWidth / 2 - MeasureText(title, fontSize) / 2 + 1, screenHeight / 2 - 400 + 1, fontSize, BLACK);
    DrawText(title, screenWidth / 2 - MeasureText(title, fontSize) / 2 + 1, screenHeight / 2 - 400 + 2, fontSize, BLACK);
    DrawText(title, screenWidth / 2 - MeasureText(title, fontSize) / 2 + 2, screenHeight / 2 - 400 + 1, fontSize, BLACK);
    DrawText(title, screenWidth / 2 - MeasureText(title, fontSize) / 2 + 2, screenHeight / 2 - 400 + 2, fontSize, BLACK);
    
    DrawText(title, screenWidth / 2 - MeasureText(title, fontSize) / 2, screenHeight / 2 - 400, fontSize, YELLOW);
    DrawText(subtitle, screenWidth / 2 - MeasureText(subtitle, subFontSize) / 2, screenHeight / 2 - 250, subFontSize, WHITE);
    DrawText(score.c_str(), screenWidth / 2 - MeasureText(score.c_str(), subFontSize) / 2, screenHeight / 2 - 200, subFontSize, WHITE);
    DrawText(highScore.c_str(), screenWidth / 2 - MeasureText(highScore.c_str(), subFontSize) / 2, screenHeight / 2 - 150, subFontSize, WHITE);
    DrawText(prompt, screenWidth / 2 - MeasureText(prompt, promptFontSize) / 2, screenHeight - 60, promptFontSize, BLACK);
}