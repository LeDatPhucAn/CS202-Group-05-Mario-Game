#include "Game.hpp"
#include "UI.hpp"
#include "MarioState.hpp"
#include <chrono>
#include <thread>
#include "BlockState.hpp"
vector<Particle> Game::particles = {};
b2World *Game::world = new b2World({0, fallGravity});
vector<Enemy *> Game::enemies = {};

Game::Game(SceneManager *_mag) : Mario(),
                                 Goomba(),
                                 Koopa(),
                                 PiranhaPlant(),
                                 Lakitu()
{
    manager = _mag;
    mapPaths = {
        {"Map1.1", "assets/Map/Map1.1.json"},
        // Add the rest...
    };
    Mario.setPosition({100, 50});
    Goomba.setPosition({150, 0});
    Koopa.setPosition({170, 0});
    PiranhaPlant.setPosition({20, 90});
    Lakitu.setPosition({50, -20});
    init();
}

void Game::init()
{

    current_Map = "Map1.1";
    curMap.choose(mapPaths[current_Map]);

    world = new b2World({0, fallGravity});

    contactListener = new ContactListener();
    world->SetContactListener(contactListener);

    for (auto &block : curMap.tileBlocks)
    {
        block->changeState(new BlockIdleState(block));
        block->createBody(world);
    }

    Mario.changeState(new IdleState(&Mario));

    Goomba.changeState(new EnemyWalkState(&Goomba));
    Koopa.changeState(new EnemyWalkState(&Koopa));
    PiranhaPlant.changeState(new EnemyIdleState(&PiranhaPlant));
    Lakitu.changeState(new EnemyIdleState(&Lakitu));
    Lakitu.setTarget(&Mario, this);

    addEnemy(&Goomba);
    addEnemy(&Koopa);
    addEnemy(&PiranhaPlant);
    addEnemy(&Lakitu);

    Mario.createBody(world);
}

void Game::addEnemy(Enemy *enemy)
{

    if (enemy)
    {
        enemy->createBody(world);
        enemies.push_back(enemy);
    }
}
void Game::removeEnemy(Enemy *enemy)
{
    if (enemy)
    {
        auto it = std::remove(enemies.begin(), enemies.end(), enemy);
        if (it != enemies.end())
        {
            enemies.erase(it, enemies.end());
            world->DestroyBody(enemy->getBody());
            enemy->attachBody(nullptr);
        }
    }
}
void Game::updateScene()
{
    // Check for pause input (P key or mouse click on pause icon)
    if (IsKeyPressed(KEY_P)) {
        manager->changeScene(sceneType::PAUSE);
        return;
    }
    
    // Check for mouse click on pause icon
    Vector2 mousePos = GetMousePosition();
    float iconSize = 40.0f;
    float iconX = UI::screenWidth - iconSize - 20; // Top-right corner with margin
    float iconY = 20;
    Rectangle pauseIconRect = {iconX, iconY, iconSize, iconSize};
    
    if (CheckCollisionPointRec(mousePos, pauseIconRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        manager->changeScene(sceneType::PAUSE);
        return;
    }
    // Step the world
    if (world) // 60 fps
        world->Step(1.0f / 60.0f, 6, 2);

    updateCharacters();

    updateMap();
}
void Game::updateCharacters()
{
    Mario.update();

    for (Enemy *enemy : enemies)
    {
        if (enemy)
        {
            enemy->update();
        }
    }
}
void Game::updateMap()
{
    curMap.update();

    for (auto &x : particles)
        x.update();

    auto &blocks = curMap.tileBlocks;
    vector<Block *> toDelete;

    blocks.erase(
        std::remove_if(blocks.begin(), blocks.end(), [&](Block *block)
                       {
        if (block->needDeletion) {
            toDelete.push_back(block);
            world->DestroyBody(block->getBody());
            return true; // mark for removal
        }
        return false; }),
        blocks.end());

    for (Block *block : toDelete)
    {
        block->behavior->block = nullptr;
        delete block;
        block = nullptr;
    }
}

void Game::displayScene()
{
    curMap.display();
    Mario.display();
    for (auto &enemy : enemies)
    {
        if (enemy && !enemy->beCleared)
        {
            enemy->display();
        }
    }
    float dt = GetFrameTime();

    for (auto &x : particles)
    {
        x.display(dt);
    }

    // Draw pause button in top-right corner
    float iconSize = 40.0f;
    float iconX = UI::screenWidth - iconSize - 20;
    float iconY = 20;
    Rectangle pauseIconRect = {iconX, iconY, iconSize, iconSize};
    
    // Check if mouse is hovering over the icon
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, pauseIconRect);
    
    // Draw background circle
    Color bgColor = isHovered ? Color{101, 67, 33, 255} : ORANGE; // Dark brown when hovered, orange when normal
    DrawCircle(iconX + iconSize/2, iconY + iconSize/2, iconSize/2, bgColor);
    
    // Draw border
    DrawCircleLines(iconX + iconSize/2, iconY + iconSize/2, iconSize/2, BLACK);
    
    // Draw pause symbol (two vertical bars)
    float barWidth = 6;
    float barHeight = iconSize * 0.5f;
    float barY = iconY + (iconSize - barHeight) / 2;
    float leftBarX = iconX + iconSize * 0.3f;
    float rightBarX = iconX + iconSize * 0.6f;
    
    DrawRectangle(leftBarX, barY, barWidth, barHeight, WHITE);
    DrawRectangle(rightBarX, barY, barWidth, barHeight, WHITE);

    //Draw Instructions
    string instructions = "Use Arrow Keys/ WASD to move - P to pause";
    Vector2 instrSize = MeasureTextEx(UI::font, instructions.c_str(), 16, 2);
    float instrX = (UI::screenWidth - instrSize.x) / 2;
    float instrY = UI::screenHeight - instrSize.y - 20;
    
    DrawTextEx(UI::font, instructions.c_str(), 
              {instrX + 1, instrY + 1}, 20, 2, WHITE);
    DrawTextEx(UI::font, instructions.c_str(), 
              {instrX, instrY}, 20, 2, BLACK);
}

Game::~Game()
{
    delete world;
    delete contactListener;
}