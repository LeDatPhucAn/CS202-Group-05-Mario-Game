#include "Game.hpp"
#include "UI.hpp"


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
    )
{
    init();
}
void Game::init() {
    //Load assets
    //Load Mari
    current_Map = "map1";
    Mario.changeState(new IdleState(&Mario));
}
void Game::updateScene() {
    Mario.update();
}
void Game::displaySceneInCamera() {
    DrawTiled(UI::gameMap[current_Map], 0, 0, WHITE);
    Mario.display();
}
void Game::displayScene() {
}
