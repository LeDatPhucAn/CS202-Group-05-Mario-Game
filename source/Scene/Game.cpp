#include "Game.hpp"
#include "UI.hpp"
#include "MarioState.hpp"
#include "Pause.hpp"
#include <chrono>
#include <thread>
#include "BlockState.hpp"
vector<Particle> Game::particles = {};
b2World *Game::world = new b2World({0, fallGravity});
vector<GameObject *> Game::gameObjects = {};
Game::Game(SceneManager *_mag)
{
    manager = _mag;
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
    goomba = new Goomba();
    koopa = new Koopa();
    piranhaPlant = new PiranhaPlant();
    lakitu = new Lakitu();

    // Set initial positions
    mario->setPosition({100, 50});
    goomba->setPosition({150, 0});
    koopa->setPosition({170, 0});
    piranhaPlant->setPosition({20, 90});
    lakitu->setPosition({50, -20});

    // Load map
    current_Map = "Map1.1";
    curMap.choose(mapPaths[current_Map]);

    // Initialize Box2D world and contact listener
    world = new b2World({0, fallGravity});
    contactListener = new ContactListener();
    world->SetContactListener(contactListener);

    // Prepare map blocks
    for (auto &block : curMap.tileBlocks)
    {
        block->changeState(new BlockIdleState(block));
        block->createBody(world);
    }

    // Initialize character states
    mario->changeState(new IdleState(mario));
    goomba->changeState(new EnemyWalkState(goomba));
    koopa->changeState(new EnemyWalkState(koopa));
    piranhaPlant->changeState(new EnemyIdleState(piranhaPlant));
    lakitu->changeState(new EnemyIdleState(lakitu));
    lakitu->setTarget(mario, this);

    // Add enemies to game
    addGameObject(goomba);
    addGameObject(koopa);
    addGameObject(piranhaPlant);
    addGameObject(lakitu);
    addGameObject(mario);

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
        gameObject->createBody(world);
        gameObjects.push_back(gameObject);
    }
}
void Game::removeGameObject(GameObject *gameObject)
{
    if (gameObject)
    {
        auto it = std::remove(gameObjects.begin(), gameObjects.end(), gameObject);
        if (it != gameObjects.end())
        {
            gameObjects.erase(it, gameObjects.end());
            world->DestroyBody(gameObject->getBody());
            gameObject->attachBody(nullptr);
        }
    }
}
void Game::updateScene()
{

    if (IsKeyPressed(KEY_P))
        manager->changeScene(sceneType::PAUSE);

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

    for (GameObject *obj : gameObjects)
    {
        if (obj)
        {
            obj->update();
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
    for (GameObject *obj : gameObjects)
    {
        if (obj)
        {
            obj->display();
        }
    }

    float dt = GetFrameTime();

    for (auto &x : particles)
    {
        x.display(dt);
    }
}

Game::~Game()
{

    delete world;
    delete contactListener;
    for (auto &obj : gameObjects)
    {
        if (obj)
            delete obj;
    }
}