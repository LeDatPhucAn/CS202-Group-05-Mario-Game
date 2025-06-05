#include "../header/Game.hpp"
#include "../header/UI.hpp"
Game::Game() 
    : Mario(
        Character::Builder()
        .setIdleFrames(9,16)
        .setWalkFrames(24,29)
        .setJumpFrames(17,20)
        .setFallFrames(21,21)
        .setJsonAndTexture("Mario3D")
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



