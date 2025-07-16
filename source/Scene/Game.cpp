#include "Game.hpp"
#include "UI.hpp"
#include "MarioState.hpp"
#include <chrono>
#include <thread>

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
                       .build())
//    ,Goomba(),
//    Koopa()
{
    mapPaths = {
        {"Map1.1", "assets/Map/Map1.1.json"},
        // Add the rest...
    };
    Mario.setPosition({100, 0});
    // Goomba.setPosition({150, 0});
    // Koopa.setPosition({175, 0});
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

    // enemies.push_back(&Goomba);
    // enemies.push_back(&Koopa);

    // for (Enemy *enemy : enemies)
    // {
    //     enemy->createBody(world);
    // }

    Mario.createBody(world);

    Mario.changeState(new IdleState(&Mario));

    // Goomba.changeState(new EnemyWalkState(&Goomba));
    // Koopa.changeState(new EnemyWalkState(&Koopa));
}

void Game::updateScene()
{
    // Step the world
    if (world) // 60 fps
        world->Step(1.0f / 60.0f, 6, 2);
    Mario.update();
    // Goomba.update();
    // Koopa.update();
    curMap.update();
}
void Game::displaySceneInCamera()
{
    curMap.display();
    Mario.display();

    // if (!Goomba.beCleared)
    // {
    //     Goomba.display();
    // }
    // if (!Koopa.beCleared)
    // {
    //     Koopa.display();
    // }
}
void Game::displayScene()
{
}

Game::~Game()
{
    delete world;
    delete contactListener;
}