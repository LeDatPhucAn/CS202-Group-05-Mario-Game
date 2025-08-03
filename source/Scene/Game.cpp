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
    spawner = new Spawner(this);
    curMap.setSpawner(spawner);
    mapPaths = {
        {"Map1.1", "assets/Map/Map1.1.json"},
        // Add the rest...
    };

    init();
}

void Game::init()
{

    // Instantiate main characters
    mario = new Mario();

    // Set initial positions
    mario->setPosition({100, 50});

    // Load map
    current_Map = "Map1.1";
    curMap.choose(mapPaths[current_Map]);

    // Initialize Box2D world and contact listener
    world = new b2World({0, fallGravity});
    contactListener = new ContactListener();
    world->SetContactListener(contactListener);
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
    cam.zoom = static_cast<float>(screenHeight) / WorldHeight;
    cam.rotation = 0;
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
void Game::removeGameObject(GameObject *gameObject)
{
    if (gameObject)
    {
        auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
        if (it != gameObjects.end())
        {
            gameObjects.erase(it);
            if (gameObject->getBody())
            {
                world->DestroyBody(gameObject->getBody());
                gameObject->attachBody(nullptr);
            }
            delete gameObject;
            gameObject = nullptr;
        }
    }
}
void Game::updateScene()
{

    if (IsKeyPressed(KEY_P))
        manager->changeScene(sceneType::PAUSE);
    if (IsKeyPressed(KEY_Q))
        showDebugDraw = !showDebugDraw;

    // Step the world
    if (world) // 60 fps
        world->Step(1.0f / 60.0f, 6, 2);

    updateCharacters();
    updateMap();

    // amera
    float delta = (float)mario->getPosition().x - prePosX;
    if (GetWorldToScreen2D(mario->getPosition(), cam).x > 0.8 * screenWidth)
        cam.target.x += (delta > 1) ? delta : 0;
    if (GetWorldToScreen2D(mario->getPosition(), cam).x < 0.2 * screenWidth)
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

    for (Block *&block : toDelete)
    {
        block->behavior->block = nullptr;
        delete block;
        block = nullptr;
    }
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
    curMap.clearAll();
    gameObjects.clear();
    delete world;
    world = nullptr;
    delete drawDebug;
    drawDebug = nullptr;
    delete contactListener;
    contactListener = nullptr;
}