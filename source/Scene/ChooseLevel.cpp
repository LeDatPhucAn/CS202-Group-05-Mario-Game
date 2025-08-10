#include "ChooseLevel.hpp"
#include "raylib.h"
#include "UI.hpp"
#include "GameInfo.hpp"
#include <algorithm>
#include <string>
ChooseLevel::ChooseLevel(SceneManager *_manager)
    : Scene(_manager), mario(nullptr), world(nullptr), selectedLevel(-1),
      levelSelected(false), interactionTimer(0.0f), showLevelInfo(false), hoveredLevel(-1)
{
    // Create physics world
    world = new b2World({0, 0});

    // Create Player
    mario = new PlayerMario();
    mario->setPosition({100, 350});
    mario->createBody(world);
    mario->isGrounded = true;

    // Load textures
    backgroundTexture = LoadTexture("assets/Backgrounds/MenuBackground.png");
    if (backgroundTexture.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load background texture");
    }

    LevelTextures.push_back(LoadTexture("assets/Backgrounds/Levels/Lv1.png"));
    LevelTextures.push_back(LoadTexture("assets/Backgrounds/Levels/Lv2.png"));
    LevelTextures.push_back(LoadTexture("assets/Backgrounds/Levels/Lv3.png"));
    if (LevelTextures[0].id == 0 || LevelTextures[1].id == 0 || LevelTextures[2].id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load level textures");
    }

    // Initialize camera exactly like Game.cpp
    cam.offset = {0, 0};
    cam.target = {0, 150};
    cam.zoom = static_cast<float>(UI::screenHeight) / WorldHeight;
    cam.rotation = 0;

    // Initialize mario's prePosX like Game.cpp
    prePosX = 100.0f;

    // Initialize level portals
    initializePortals();
    SoundController::getInstance().playSceneMusicFromStart(sceneType::CHOOSE_LEVEL);
    instructionText = "Use ARROW KEYS to move Player - Walk into rectangles to select level - ESC to go back";
}

ChooseLevel::~ChooseLevel()
{
    if (mario)
    {
        delete mario;
    }
    if (world)
    {
        delete world;
    }

    // Unload textures
    if (backgroundTexture.id > 0)
        UnloadTexture(backgroundTexture);
}

void ChooseLevel::updateScene()
{
    float deltaTime = GetFrameTime();
    interactionTimer += deltaTime;

    // Handle ESC to go back
    if (IsKeyPressed(KEY_P))
    {
        manager->goBack();
        return;
    }

    // Step the world exactly like Game.cpp
    if (world) // 60 fps
        world->Step(1.0f / 60.0f, 6, 2);

    // Make sure Player doesn't go too far
    mario->update();
    if (mario->getPosition().y > 350.0f)
    {
        mario->setPosition({mario->getPosition().x, 350.0f});
        mario->body->SetTransform(b2Vec2(mario->getPosition().x / PPM, 350.0f / PPM), 0);
        mario->isGrounded = true;
        mario->body->SetLinearVelocity({mario->body->GetLinearVelocity().x, 0});

        // Force Player back to idle state if he was falling
        if (dynamic_cast<FallState *>(mario->currentState))
        {
            mario->changeState(new IdleState(mario));
        }
    }

    if (mario->getPosition().x < 50)
    {
        mario->setPosition({50, mario->getPosition().y});
        mario->body->SetTransform(b2Vec2(50 / PPM, mario->getPosition().y / PPM), 0);
    }
    if (mario->getPosition().x > 800)
    {
        mario->setPosition({800, mario->getPosition().y});
        mario->body->SetTransform(b2Vec2(800 / PPM, mario->getPosition().y / PPM), 0);
    }

    // Camera logic exactly like Game.cpp
    float delta = (float)mario->getPosition().x - prePosX;
    Vector2 screenPos = GetWorldToScreen2D(mario->getPosition(), cam);

    // Always keep Player roughly centered
    if (screenPos.x > 0.6 * UI::screenWidth || screenPos.x < 0.4 * UI::screenWidth)
        cam.target.x += delta;

    prePosX = mario->getPosition().x;

    // Check portal collisions
    checkPortalCollisions();

    // Auto-select level when Player touches a rectangle
    if (selectedLevel != -1)
    {
        // Start the transition timer when a level is first selected
        static float transitionTimer = 0.0f;
        static int lastSelectedLevel = -1;

        // Reset timer if a different level is selected
        if (lastSelectedLevel != selectedLevel)
        {
            transitionTimer = 0.0f;
            lastSelectedLevel = selectedLevel;
        }

        // Increment timer
        transitionTimer += deltaTime;

        // Wait 1 seconds before switching
        if (transitionTimer >= 0.7f)
        {
            // GameInfo::getInstance()->setCurrentLevel(levelPortals[selectedLevel].levelNumber);
            manager->curMap = "Map" + to_string(selectedLevel + 1);
            manager->changeScene(sceneType::PRE_GAME);
            transitionTimer = 0.0f; // Reset for next time
            return;
        }
    }
    else
    {
        // Reset timer if no level is selected
        static float transitionTimer = 0.0f;
        static int lastSelectedLevel = -1;
        transitionTimer = 0.0f;
        lastSelectedLevel = -1;
    }
}

void ChooseLevel::initializePortals()
{
    // Create only 3 level rectangles at Player's feet level
    levelPortals.clear();

    // Level 1-1
    LevelPortal level1;
    level1.position = {300, 325}; // World coordinates like Game.cpp
    level1.levelNumber = 1;
    level1.bounds = {level1.position.x - 50, level1.position.y - 60, 100, 60};
    level1.levelName = "World 1-1";
    levelPortals.push_back(level1);

    // Level 1-2
    LevelPortal level2;
    level2.position = {500, 325};
    level2.levelNumber = 2;
    level2.bounds = {level2.position.x - 50, level2.position.y - 60, 100, 60};
    level2.levelName = "World 1-2";
    levelPortals.push_back(level2);

    // Level 1-3
    LevelPortal level3;
    level3.position = {700, 325};
    level3.levelNumber = 3;
    level3.bounds = {level3.position.x - 50, level3.position.y - 60, 100, 60};
    level3.levelName = "World 1-3";
    levelPortals.push_back(level3);
}

void ChooseLevel::checkPortalCollisions()
{
    Vector2 marioPos = mario->getPosition();
    Rectangle marioRect = {marioPos.x - 16, marioPos.y - 16, 32, 32}; // Normal Player collision

    selectedLevel = -1;
    hoveredLevel = -1;
    showLevelInfo = false;

    for (int i = 0; i < levelPortals.size(); i++)
    {
        // Check if Player's rectangle overlaps with portal rectangle
        if (CheckCollisionRecs(marioRect, levelPortals[i].bounds))
        {
            selectedLevel = i;
            hoveredLevel = i;
            showLevelInfo = true;
            break;
        }
    }
}

void ChooseLevel::displayScene()
{
    // Use Game.cpp's display pattern with camera
    BeginMode2D(cam);

    // Draw tiled background using GetScreenToWorld2D
    if (backgroundTexture.id > 0)
    {
        // Get world coordinates of screen corners
        Vector2 topLeft = GetScreenToWorld2D({0, 0}, cam);
        Vector2 bottomRight = GetScreenToWorld2D({(float)UI::screenWidth, (float)UI::screenHeight}, cam);

        // Calculate how many tiles we need
        int startTileX = (int)floor(topLeft.x / backgroundTexture.width);
        int endTileX = (int)ceil(bottomRight.x / backgroundTexture.width);
        int startTileY = (int)floor(topLeft.y / backgroundTexture.height);
        int endTileY = (int)ceil(bottomRight.y / backgroundTexture.height);

        // Draw tiles to cover the visible area
        for (int x = startTileX; x <= endTileX; x++)
        {
            for (int y = startTileY; y <= endTileY; y++)
            {
                DrawTexture(backgroundTexture,
                            x * backgroundTexture.width,
                            y * backgroundTexture.height,
                            WHITE);
            }
        }
    }
    else
    {
        ClearBackground(SKYBLUE);
    }

    // Draw simple ground line
    DrawLine(0, 380, 1000, 380, BROWN);

    // Draw level rectangles
    drawPortals();

    // Draw Player exactly like Game.cpp
    mario->display();

    EndMode2D();

    // Draw UI outside camera context
    drawUI();
}

void ChooseLevel::drawPortals()
{
    for (int i = 0; i < levelPortals.size(); i++)
    {
        const LevelPortal &portal = levelPortals[i];

        // Use level texture if available, otherwise draw colored rectangle
        if (i < LevelTextures.size() && LevelTextures[i].id > 0)
        {
            // Calculate scale to fit image within portal bounds
            float scaleX = portal.bounds.width / (float)LevelTextures[i].width;
            float scaleY = portal.bounds.height / (float)LevelTextures[i].height;
            float scale = fmin(scaleX, scaleY); // Use smaller scale to fit entirely within bounds

            // Calculate scaled dimensions
            float scaledWidth = LevelTextures[i].width * scale;
            float scaledHeight = LevelTextures[i].height * scale;

            // Center the scaled image within the portal bounds
            Vector2 imagePos = {portal.bounds.x + (portal.bounds.width - scaledWidth) / 2.0f,
                                portal.bounds.y + (portal.bounds.height - scaledHeight) / 2.0f};

            // Add highlight effect if hovered
            Color tint = (hoveredLevel == i) ? Color{255, 255, 0, 200} : WHITE; // Yellow tint when hovered
            DrawTextureEx(LevelTextures[i], imagePos, 0, scale, tint);

            // Optional: Draw border if hovered
            if (hoveredLevel == i)
            {
                DrawRectangleLinesEx(portal.bounds, 4, YELLOW);
            }
        }
        else
        {
            // Fallback: Draw colored rectangle if texture not available
            Color portalColor = GREEN;
            if (hoveredLevel == i)
            {
                portalColor = YELLOW;
            }

            // Draw rectangle
            DrawRectangleRec(portal.bounds, portalColor);
            DrawRectangleLinesEx(portal.bounds, 3, BLACK);
        }

        // Draw level number on top of the image/rectangle
        std::string levelText = std::to_string(portal.levelNumber);
        Vector2 textSize = MeasureTextEx(UI::boldFont, levelText.c_str(), 32, 2);
        Vector2 textPos = {portal.position.x - textSize.x / 2, portal.position.y};

        DrawTextEx(UI::boldFont, levelText.c_str(), {textPos.x + 2, textPos.y + 2}, 32, 2, BLACK);
        DrawTextEx(UI::boldFont, levelText.c_str(), textPos, 32, 2, WHITE);
    }
}

void ChooseLevel::drawPlayer()
{
    // Just use mario->display() like Game.cpp
    mario->display();
}

void ChooseLevel::drawUI()
{
    // Draw level info panel (using screen coordinates, not world coordinates)
    if (showLevelInfo && hoveredLevel != -1)
    {
        const LevelPortal &portal = levelPortals[hoveredLevel];

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

    // Draw instructions at bottom (screen coordinates)
    Vector2 instrSize = MeasureTextEx(UI::font, instructionText.c_str(), 16, 2);
    float instrX = (UI::screenWidth - instrSize.x) / 2;
    float instrY = UI::screenHeight - 30;

    DrawTextEx(UI::font, instructionText.c_str(),
               {instrX + 1, instrY + 1}, 16, 2, BLACK);
    DrawTextEx(UI::font, instructionText.c_str(),
               {instrX, instrY}, 16, 2, WHITE);
}