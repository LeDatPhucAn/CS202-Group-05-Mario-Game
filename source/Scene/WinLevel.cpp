#include "WinLevel.hpp"
#include "Menu.hpp"
#include "Scene.hpp"
#include "Game.hpp"
#include <raylib.h>

WinLevel::WinLevel(SceneManager* manager)
    : Scene(manager), anyKeyPressed(false), frameCount(0), currentFrame(0), 
      frameTime(0.0f), frameDuration(0.03f), gifLoaded(false) {
        mapName = manager->curMap; 
        
        // Load GIF animation
        gifImage = LoadImageAnim("assets/Backgrounds/MarioWin.gif", &frameCount);
        if (gifImage.data != nullptr && frameCount > 0) {
            gifTexture = LoadTextureFromImage(gifImage);
            gifLoaded = true;
        } else {
            TraceLog(LOG_WARNING, "Failed to load winLevel.gif");
            gifLoaded = false;
        }
    }

WinLevel::~WinLevel() {
    if (gifLoaded) {
        UnloadTexture(gifTexture);
        UnloadImage(gifImage);
    }
}

void WinLevel::updateScene() {

    if (gifLoaded && frameCount > 1) {
        float dt = GetFrameTime();
        frameTime += dt;
        if (frameTime >= frameDuration) {
            frameTime -= frameDuration;
            currentFrame = (currentFrame + 1) % frameCount;
            int pixelsPerFrame = gifImage.width * gifImage.height * 4; // RGBA32
            unsigned char* framePtr = ((unsigned char*)gifImage.data) + (currentFrame * pixelsPerFrame);
            UpdateTexture(gifTexture, framePtr);
        }
    }

    if (!anyKeyPressed && GetKeyPressed()!=0) {
        anyKeyPressed = true;
        if (mapName == "Map1" || mapName == "Map2") {
            // Move to next level
            if (manager->curMap == "Map1") manager->curMap = "Map2";
            else if (manager->curMap == "Map2") manager->curMap = "Map3";
            manager->shouldReloadGame = true;
            manager->goBack();
        } else if (mapName == "Map3") {
            manager->goBackOfBaseScene(); // This should go to main menu
        }
    }
}



void WinLevel::displayScene() {
    ClearBackground(BLACK);
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int fontSize = 60;
    int subFontSize = 30;
    int promptFontSize = 24;
    
    const char* title = "";
    const char* subtitle = "";
    const char* prompt = "";
    static string titleStr = "";

    if (mapName == "Map1" || mapName == "Map2") {
        titleStr = (mapName == "Map1") ? "Level 1 Completed" : "Level 2 Completed";
        title = titleStr.c_str();
        subtitle = "You almost saved the princess, keep going";
        prompt = "Press any key to move to next level";
        
        if (gifLoaded) {
            float scale = 0.5f; // Make Mario 50% smaller
            int scaledWidth = (int)(gifTexture.width * scale);
            int scaledHeight = (int)(gifTexture.height * scale);
            int gifX = screenWidth/2 - scaledWidth/2;
            int gifY = screenHeight/2 + 60;
            
            Rectangle source = {0, 0, (float)gifTexture.width, (float)gifTexture.height};
            Rectangle dest = {(float)gifX, (float)gifY, (float)scaledWidth, (float)scaledHeight};
            Vector2 origin = {0, 0};
            
            DrawTexturePro(gifTexture, source, dest, origin, 0.0f, WHITE);
        }
    } else if (mapName == "Map3") {
        title = "Victory";
        subtitle = "You have saved the princess";
        prompt = "Press any key to back to the menu";
    }
    

    DrawText(title, screenWidth/2 - MeasureText(title, fontSize)/2, screenHeight/2 - 200, fontSize, YELLOW);
    DrawText(subtitle, screenWidth/2 - MeasureText(subtitle, subFontSize)/2, screenHeight/2 - 80, subFontSize, WHITE);
    DrawText(prompt, screenWidth/2 - MeasureText(prompt, promptFontSize)/2, screenHeight/2, promptFontSize, GRAY);
}