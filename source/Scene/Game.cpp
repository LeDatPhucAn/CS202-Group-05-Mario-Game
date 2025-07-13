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
               Koopa()
{
    mapPaths = {
        {"Map1.1", "assets/Map/Map1.1.json"},
        // Add the rest...
    };
    init();
    Mario.setPosition({100, 0});
    Goomba.setPosition({150, 0});
    Koopa.setPosition({175, 0});
}
void Game::init()
{
    current_Map = "Map1.1";
    curMap.choose(mapPaths[current_Map]);

    Mario.changeState(new IdleState(&Mario));

    Goomba.changeState(new EnemyWalkState(&Goomba));
    Koopa.changeState(new EnemyWalkState(&Koopa));

    // initialize Collision Manager
    enemies.push_back(&Goomba);
    enemies.push_back(&Koopa);
    // characters.push_back(&Goomba);
    collisionManager.init(&curMap, &Mario, enemies);
}

void Game::updateScene()
{
    Mario.update();
    Goomba.update();
    Koopa.update();
    curMap.update();
    // collision
    collisionManager.ManageCollision();

    // Mario with Goomba & Koopa
    if (Mario.checkCollision(&Goomba))
    {
        Mario.updateCollision(&Goomba, 1);
        Goomba.updateCollision(&Mario, 1);
    }

    if (Mario.checkCollision(&Koopa))
    {
        Mario.updateCollision(&Koopa, 1);
        Koopa.updateCollision(&Mario, 1);
    }

    if (Goomba.checkCollision(&Koopa))
    {
        Goomba.updateCollision(&Koopa, 1);
        Koopa.updateCollision(&Goomba, 1);
    }
}
void Game::displaySceneInCamera()
{
    curMap.display();
    Mario.display();

    if (!Goomba.beCleared)
    {
        Goomba.display();
    }
    if (!Koopa.beCleared)
    {
        Koopa.display();
    }
}
void Game::displayScene()
{
}
