#include "Game.hpp"
#include "UI.hpp"
#include "MarioState.hpp"
#include <chrono>
#include <thread>

vector<Particle> Game::particles = {};
b2World *Game::world = new b2World({0, fallGravity});
vector<Enemy *> Game::enemies = {};
Game::Game() : Mario(),
               Goomba(),
               Koopa(),
               PiranhaPlant(),
               Lakitu()
{
    mapPaths = {
        {"Map1.1", "assets/Map/Map1.1.json"},
        // Add the rest...
    };
    Mario.setPosition({500, 50});
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
    // addEnemy(&Lakitu);

    Mario.createBody(world);
}

void Game::addEnemy(Enemy *enemy)
{

    if (enemy)
    {
        std::cout << "enemy pointer: " << enemy << std::endl;
        std::cout << "enemy type: " << typeid(*enemy).name() << std::endl;

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
void Game::displaySceneInCamera()
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
}
void Game::displayScene()
{
}

Game::~Game()
{
    delete world;
    delete contactListener;
}