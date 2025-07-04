#include "Game.hpp"
#include "UI.hpp"
#include "MarioState.hpp"
Game::Game(): Mario(
        Character::Builder()
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
        .setPos({ 100,0 })
        .build()
    ),
    Goomba(
        Enemy::Builder()
        .setJsonAndTexture("Enemies2D")
        .setFrames(IDLE, 0, 0)
        .setFrames(WALK, 0, 1)
        .setFrames(JUMP, 0, 0)
        .setFrames(FALL, 0, 1)
        .setPos({ 150,0 })
        .build()
    )
{
    mapPaths = {
		{"Map1.1", "assets/Map/Map1.1.json"},
		// Add the rest...
	};
    init();
}
void Game::init() {
    //Load assets
    //Load Mari
    current_Map = "Map1.1";
    curMap.choose(mapPaths[current_Map]);

    Mario.changeState(new IdleState(&Mario));
    Goomba.changeState(new IdleState(&Goomba));
}
void Game::updateScene() {
    Mario.update();
    Goomba.update();
}
void Game::displaySceneInCamera() {
    curMap.display();
    Mario.display();
    Goomba.display();
}
void Game::displayScene() {
}