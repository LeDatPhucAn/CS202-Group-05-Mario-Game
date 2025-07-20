#include "Game.hpp"
#include "UI.hpp"
#include "MarioState.hpp"
#include <chrono>
#include <thread>

std::vector<Particle> Game::particles = {};

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

    enemies.push_back(&Goomba);
    enemies.push_back(&Koopa);
    enemies.push_back(&PiranhaPlant);
    enemies.push_back(&Lakitu);

    for (Enemy *enemy : enemies)
    {
        addEnemy(enemy);
    }

    Mario.createBody(world);

    Mario.changeState(new IdleState(&Mario));

    Goomba.changeState(new EnemyWalkState(&Goomba));
    Koopa.changeState(new EnemyWalkState(&Koopa));
    PiranhaPlant.changeState(new EnemyIdleState(&PiranhaPlant));
    Lakitu.changeState(new EnemyIdleState(&Lakitu));
    Lakitu.setTarget(&Mario, this);
}

void Game::addEnemy(Enemy *newEnemy)
{
    if (newEnemy)
    {
        newEnemy->createBody(world);
        enemies.push_back(newEnemy);
    }
}

void Game::updateScene()
{
    // Step the world
    if (world) // 60 fps
        world->Step(1.0f / 60.0f, 6, 2);

    Mario.update();
    for (Enemy *enemy : enemies)
    {
        if (enemy)
        {
            enemy->update();
        }
    }

    curMap.update();

    for (auto &x : particles)
        x.update();
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