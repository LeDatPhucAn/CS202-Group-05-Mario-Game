#include "ChooseLevel.hpp"
#include "raylib.h"
#include "UI.hpp"
#include "Score.hpp"
#include <algorithm>

ChooseLevel::ChooseLevel(SceneManager* _manager) 
    : Scene(_manager), mario(nullptr), world(nullptr), selectedLevel(-1), 
      levelSelected(false), interactionTimer(0.0f), showLevelInfo(false), hoveredLevel(-1)
{
    // Create Mario without physics body
    mario = new Mario();
    mario->setPosition({100, 300}); // Set initial position on ground
    
    // Load textures
    backgroundTexture = LoadTexture("assets/Backgrounds/LevelSelect.png");
    groundTexture = LoadTexture("assets/Tiles/Ground.png");
    portalTexture = LoadTexture("assets/Objects/Portal.png");
    
    // Initialize camera
    camera.target = {mario->getPosition().x, mario->getPosition().y};
    camera.offset = {UI::screenWidth / 2.0f, UI::screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    // Initialize level portals (simple rectangles)
    initializePortals();
    
    instructionText = "Use ARROW KEYS to move Mario - Touch rectangles to select level - ESC to go back";
}

ChooseLevel::~ChooseLevel()
{
    if (mario) {
        delete mario;
    }
    
    // Unload textures
    if (backgroundTexture.id > 0) UnloadTexture(backgroundTexture);
    if (groundTexture.id > 0) UnloadTexture(groundTexture);
    if (portalTexture.id > 0) UnloadTexture(portalTexture);
}

void ChooseLevel::initializePortals()
{
    // Create only 3 level rectangles at Mario's feet level
    levelPortals.clear();
    
    // Level 1-1
    LevelPortal level1;
    level1.position = {300, 350}; // At ground level
    level1.levelNumber = 1;
    level1.bounds = {level1.position.x - 50, level1.position.y - 30, 100, 60};
    level1.levelName = "World 1-1";
    levelPortals.push_back(level1);
    
    // Level 1-2
    LevelPortal level2;
    level2.position = {500, 350}; // At ground level
    level2.levelNumber = 2;
    level2.bounds = {level2.position.x - 50, level2.position.y - 30, 100, 60};
    level2.levelName = "World 1-2";
    levelPortals.push_back(level2);
    
    // Level 1-3
    LevelPortal level3;
    level3.position = {700, 350}; // At ground level
    level3.levelNumber = 3;
    level3.bounds = {level3.position.x - 50, level3.position.y - 30, 100, 60};
    level3.levelName = "World 1-3";
    levelPortals.push_back(level3);
}

void ChooseLevel::updateScene()
{
    float deltaTime = GetFrameTime();
    interactionTimer += deltaTime;
    
    // Handle ESC to go back
    if (IsKeyPressed(KEY_ESCAPE))
    {
        manager->goBack();
        return;
    }
    
    // Handle Mario movement (simple 2D movement, no physics)
    Vector2 marioPos = mario->getPosition();
    float moveSpeed = 200.0f * deltaTime; // pixels per second
    
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        marioPos.x -= moveSpeed;
        mario->direction = LEFT;
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        marioPos.x += moveSpeed;
        mario->direction = RIGHT;
    }
    
    // Keep Mario on the ground level
    marioPos.y = 300;
    mario->setPosition(marioPos);
    
    // Update camera
    updateCamera();
    
    // Check portal collisions
    checkPortalCollisions();
    
    // Auto-select level when Mario touches a rectangle
    if (selectedLevel != -1)
    {
        // Set the selected level
        // Score::getInstance()->setCurrentLevel(levelPortals[selectedLevel].levelNumber);
        
        // Transition to game scene with selected level
        manager->changeScene(sceneType::GAME);
        return;
    }
}

void ChooseLevel::updateCamera()
{
    // Follow Mario with the camera
    Vector2 marioPos = mario->getPosition();
    
    // Smooth camera following
    float lerpAmount = 2.0f * GetFrameTime();
    camera.target.x = Lerp(camera.target.x, marioPos.x, lerpAmount);
    camera.target.y = Lerp(camera.target.y, marioPos.y - 50, lerpAmount); // Offset camera slightly up
    
    // Keep camera within bounds (reduced since only 3 levels)
    float minX = UI::screenWidth / 2.0f;
    float maxX = 800 - UI::screenWidth / 2.0f; // Based on last portal position (700 + 100 margin)
    camera.target.x = Clamp(camera.target.x, minX, maxX);
}

void ChooseLevel::checkPortalCollisions()
{
    Vector2 marioPos = mario->getPosition();
    Rectangle marioRect = {marioPos.x - 32, marioPos.y - 32, 64, 64}; // Bigger Mario collision box (4x)
    
    selectedLevel = -1;
    hoveredLevel = -1;
    showLevelInfo = false;
    
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        // Check if Mario is touching any portal
        for (int i = 0; i < levelPortals.size(); i++)
        {
            if (CheckCollisionRecs(marioRect, levelPortals[i].bounds))
            {
                selectedLevel = i;
                hoveredLevel = i;
                showLevelInfo = true;
                interactionTimer = 0.0f; // Reset interaction timer
                break;
            }
        }
    }
}

void ChooseLevel::displayScene()
{
    BeginMode2D(camera);
    
    // Draw background
    if (backgroundTexture.id > 0)
    {
        DrawTexture(backgroundTexture, 0, 0, WHITE);
    }
    else
    {
        ClearBackground(SKYBLUE);
    }
    
    // Draw simple ground line
    DrawLine(0, 380, 1000, 380, BROWN); // Shorter ground line for 3 levels
    
    // Draw level rectangles
    drawPortals();
    
    // Draw Mario (4x bigger)
    drawMario();
    
    EndMode2D();
    
    // Draw UI
    drawUI();
}

void ChooseLevel::drawPortals()
{
    for (int i = 0; i < levelPortals.size(); i++)
    {
        const LevelPortal& portal = levelPortals[i];
        
        // Choose color based on selection
        Color portalColor = GREEN;
        if (hoveredLevel == i)
        {
            portalColor = YELLOW;
        }
        
        // Draw rectangle
        DrawRectangleRec(portal.bounds, portalColor);
        DrawRectangleLinesEx(portal.bounds, 3, BLACK);
        
        // Draw level number
        std::string levelText = std::to_string(portal.levelNumber);
        Vector2 textSize = MeasureTextEx(UI::boldFont, levelText.c_str(), 32, 2);
        Vector2 textPos = {portal.position.x - textSize.x/2, portal.position.y - textSize.y/2};
        
        DrawTextEx(UI::boldFont, levelText.c_str(), 
                  {textPos.x + 2, textPos.y + 2}, 32, 2, BLACK);
        DrawTextEx(UI::boldFont, levelText.c_str(), 
                  textPos, 32, 2, WHITE);
    }
}

void ChooseLevel::drawMario()
{
    if (mario && mario->sprite.texture.id > 0)
    {
        Vector2 marioPos = mario->getPosition();

        // Get current frame - using fixed frame 0
        Rectangle sourceRect = mario->sprite.frameRecs[0];
        
        // Make Mario 4x bigger
        Rectangle destRect = {
            marioPos.x - (sourceRect.width * 4) / 2, 
            marioPos.y - (sourceRect.height * 4) / 2, 
            sourceRect.width * 4, 
            sourceRect.height * 4
        };
        
        // Flip texture based on direction
        if (mario->direction == LEFT)
        {
            sourceRect.width = -abs(sourceRect.width);
        }
        
        DrawTexturePro(mario->sprite.texture, sourceRect, destRect, {0, 0}, 0.0f, WHITE);
    }
    else
    {
        // Fallback Mario drawing (4x bigger)
        Vector2 marioPos = mario->getPosition();
        DrawCircleV(marioPos, 32, RED); // 4x bigger radius (was 16, now 32)
    }
}

void ChooseLevel::drawUI()
{
    // Draw level info panel
    if (showLevelInfo && hoveredLevel != -1)
    {
        const LevelPortal& portal = levelPortals[hoveredLevel];
        
        Rectangle infoPanel = {20, 20, 300, 80};
        DrawRectangleRounded(infoPanel, 0.1f, 10, Color{0, 0, 0, 150});
        DrawRectangleRoundedLines(infoPanel, 0.1f, 10, WHITE);
        
        // Level name
        DrawTextEx(UI::boldFont, portal.levelName.c_str(), 
                  {infoPanel.x + 10, infoPanel.y + 10}, 24, 2, WHITE);
        
        // Instructions
        DrawTextEx(UI::font, "Starting level...", 
                  {infoPanel.x + 10, infoPanel.y + 45}, 18, 2, GREEN);
    }
    
    // Draw instructions at bottom
    Vector2 instrSize = MeasureTextEx(UI::font, instructionText.c_str(), 16, 2);
    float instrX = (UI::screenWidth - instrSize.x) / 2;
    float instrY = UI::screenHeight - 30;
    
    DrawTextEx(UI::font, instructionText.c_str(), 
              {instrX + 1, instrY + 1}, 16, 2, BLACK);
    DrawTextEx(UI::font, instructionText.c_str(), 
              {instrX, instrY}, 16, 2, WHITE);
}