#include "Game.hpp"
#include "UI.hpp"
#include "MarioState.hpp"
#include <chrono>
#include <thread>

CollisionManager Game::collisionManager = CollisionManager();

Game::Game() : Mario(
                   Mario::Builder()
                       .setFrames(IDLE, 0, 0)
                       .setFrames(WALK, 1, 3)
                       .setFrames(JUMP, 5, 5)
                       .setFrames(FALL, 5, 5)
                       .setFrames(SKID, 4, 4)
                       .setFrames(RUN, 1, 3)
                       .setFrames(CROUCH, 0, 0)
                       .setFrames(GROW, 44, 50)
                       .setFrames(UNGROW, 50, 44)
                       .setJsonAndTexture("Mario2D")
                       //.setPos({ 100,0 })
                       .build()),
                Goomba(),
                Koopa(),
                PiranhaPlant(),
                Lakitu()
{
    mapPaths = {
        {"Map1.1", "assets/Map/Map1.1.json"},
        // Add the rest...
    };
    init();
    Mario.setPosition({100, 0});
    Goomba.setPosition({50, 0});
    Koopa.setPosition({70, 0});
    PiranhaPlant.setPosition({330, 90});
    Lakitu.setPosition({500, 0});
}
void Game::init()
{
    current_Map = "Map1.1";
    curMap.choose(mapPaths[current_Map]);

    Mario.changeState(new IdleState(&Mario));

    Goomba.changeState(new EnemyWalkState(&Goomba));
    Koopa.changeState(new EnemyWalkState(&Koopa));
    PiranhaPlant.changeState(new EnemyIdleState(&PiranhaPlant));
    Lakitu.changeState(new EnemyIdleState(&Lakitu));
    Lakitu.setTarget(&Mario, this);

    // initialize Collision Manager
    enemies.push_back(&Goomba);
    enemies.push_back(&Koopa);
    enemies.push_back(&PiranhaPlant);
    enemies.push_back(&Lakitu);
    // characters.push_back(&Goomba);
    collisionManager.init(&curMap, &Mario, enemies);
}

void Game::addEnemy(Enemy* newEnemy)
{
    if (newEnemy)
    {
        enemies.push_back(newEnemy);
        collisionManager.init(&curMap, &Mario, enemies);
    }
}

void Game::updateScene()
{
    Mario.update();
   for (auto &enemy : enemies)
    {
        if (enemy) enemy->update();
    }
    curMap.update();
    collisionManager.ManageCollision();
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
}
void Game::displayScene()
{
}
