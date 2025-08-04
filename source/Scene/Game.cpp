#include "Game.hpp"
#include "UI.hpp"
#include "MarioState.hpp"
#include "Pause.hpp"
#include <chrono>
#include <thread>
#include "BlockState.hpp"
#include "Spawner.hpp"
#include "Projectile.hpp"
#include "DrawDebug.hpp"
vector<Particle> Game::particles = {};
b2World *Game::world = nullptr;
vector<GameObject *> Game::gameObjects = {};

Game::Game(SceneManager *_mag)
{
    manager = _mag;
    
    world = new b2World({0, fallGravity});
    contactListener = new ContactListener();
    world->SetContactListener(contactListener);

    HUDLives = LoadTexture("assets/Backgrounds/HUD/lives.png");
    if (HUDLives.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load HUD Lives texture");
    }

    HUDTime = LoadTexture("assets/Backgrounds/HUD/time.png");
    if (HUDTime.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load HUD Time texture");
    }
    
    init();
}

void Game::init()
{

    // Instantiate main charact4ers
    mario = new Mario();

    // Set initial positions
    mario->setPosition({100, 50});

    spawner = new Spawner(this);
    curMap.setSpawner(spawner);
    // Load map
    current_Map = "Map1.1";
    curMap.choose(UI::mapPaths[current_Map]);


    drawDebug = new DrawDebug();
    // Prepare map blocks
    for (auto &block : curMap.tileBlocks)
    {
        block->changeState(new BlockIdleState(block));
        block->createBody(world);
    }

    // Add enemies to game
    addGameObject(mario);
    spawner->spawnEnemy();
    // Initialize camera
    cam.offset = {0, 0};
    cam.target = {0, 0};
    cam.zoom = static_cast<float>(UI::screenHeight) / WorldHeight;
    cam.rotation = 0;

    gameTime = 0.0f;
    lives = 3;
}

Game::~Game()
{
    for (int i = 0; i < deleteLater.size(); i++)
    {
        if (deleteLater[i])
        {
            delete deleteLater[i];
            deleteLater[i] = nullptr;
        }
    }
    deleteLater.clear();

    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i])
        {
            delete gameObjects[i];
            gameObjects[i] = nullptr;
        }
    }
    gameObjects.clear();

    delete world;
    world = nullptr;

    delete drawDebug;
    drawDebug = nullptr;

    delete contactListener;
    contactListener = nullptr;

    delete spawner;
    spawner = nullptr;

    if (HUDLives.id != 0){
        UnloadTexture(HUDLives);
        HUDLives.id = 0; 
    }
    if (HUDTime.id != 0){
        UnloadTexture(HUDTime);
        HUDTime.id = 0;
    }
}

void Game::addGameObject(GameObject *gameObject)
{
    if (gameObject)
    {
        if (dynamic_cast<Projectile *>(gameObject))
            cout << "FireBall After: " << gameObject << "\n";
        gameObject->createBody(world);
        gameObjects.push_back(gameObject);
    }
}

void Game::removeGameObject()
{

    //Delete non-Block
    for (int i = 0; i < deleteLater.size(); i++)
    {
        auto it = std::find(gameObjects.begin(), gameObjects.end(), deleteLater[i]);
        if (it != gameObjects.end())
        {
            gameObjects.erase(it);
            if (deleteLater[i]->getBody())
            {
                world->DestroyBody(deleteLater[i]->getBody());
                deleteLater[i]->attachBody(nullptr);
            }
        }
    }

    //Delete Block
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

    for (Block *&block : toDelete)
    {
        block->behavior->block = nullptr;
        delete block;
        block = nullptr;
    }

}

void Game::updateScene()
{

    if (IsKeyPressed(KEY_P))
        manager->changeScene(sceneType::PAUSE);
    if (IsKeyPressed(KEY_Q))
        showDebugDraw = !showDebugDraw;

    //Update time
    gameTime += GetFrameTime();

    // Step the world
    if (world) // 60 fps
        world->Step(1.0f / 60.0f, 6, 2);

    updateCharacters();
    updateMap();

    removeGameObject();

    // Camera
    float delta = (float)mario->getPosition().x - prePosX;
    if (GetWorldToScreen2D(mario->getPosition(), cam).x > 0.5 * UI::screenWidth)
        cam.target.x += (delta > 1) ? delta : 0;
    if (GetWorldToScreen2D(mario->getPosition(), cam).x < 0.2 * UI::screenWidth)
        cam.target.x += (delta < -1) ? delta : 0;

    prePosX = mario->getPosition().x;
}

void Game::updateCharacters()
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i])
        {
            Enemy *enemy = dynamic_cast<Enemy *>(gameObjects[i]);
            if (enemy)
            {
                enemy->update(mario->getPosition());
            }
            else
                gameObjects[i]->update();

            if (gameObjects[i]->needDeletion)
            {
                gameObjects[i]->needDeletion = false;
                deleteLater.push_back(gameObjects[i]);
            }
        }
    }

}
void Game::updateMap()
{
    curMap.update();

    for (auto &x : particles)
        x.update();

   
}

void Game::displayScene()
{
    curMap.display();
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i])
        {

            gameObjects[i]->display();
        }
    }

    float dt = GetFrameTime();
    for (auto &x : particles)
    {
        x.display(dt);
    }

    if (showDebugDraw)
        drawDebug->DrawWorld(world);
    
    drawHUD();
}

void Game::drawHUD()
{
    // Calculate remaining time
    int remainingTime = (int)(maxTime - gameTime);
    if (remainingTime < 0) remainingTime = 0;
    
    
    // Draw Lives
    for (int i = 0; i < lives; i++)
    {
        Rectangle srcRec = {0, 0, (float)HUDLives.width, (float)HUDLives.height};
        Rectangle destRec = {10 + i * 15, 5, 12, 12};
        DrawTexturePro(HUDLives, srcRec, destRec, {0, 0}, 0.0f, WHITE);
    }
    
    
    
    // Draw Score (later)
    
    // Draw Time
    Rectangle srcRec = {0, 0, (float)HUDTime.width, (float)HUDTime.height};
    Rectangle destRec = {100, 3, 15, 15};
    DrawTexturePro(HUDTime, srcRec, destRec, {0, 0}, 0.0f, WHITE);
    string timeText = to_string(remainingTime);
    // Time warning - change color when time is low
    if (remainingTime <= 30)
    {
        DrawTextEx(UI::font, timeText.c_str(), {120, 7}, 12, 2, RED);
    }

    else {

        DrawTextEx(UI::font, timeText.c_str(), {120, 7}, 12, 2, WHITE);
    }
}
