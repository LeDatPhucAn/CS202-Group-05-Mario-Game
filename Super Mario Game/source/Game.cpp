#include "../header/Game.hpp"
#include "../header/UI.hpp"
Game::Game() 
    : Mario(
        Character::Builder()
        .setIdleFrames(13,13)
        .setWalkFrames(16,14)
        .setJumpFrames(18,18)
        .setFallFrames(18,18)
        .setJsonAndTexture("Mario2D")
        .setSpeed(6)
        .setPos({100,100})
        .build()
    )
{
    init();
}
void Game::init() {
    Mario.changeState(new IdleState(&Mario));
}
void Game::updateScene() {
    Mario.update();
}
void Game::displaySceneInCamera() {
    Mario.draw();
}
void Game::displayScene() {

}



