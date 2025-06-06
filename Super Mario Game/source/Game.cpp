#include "../header/Game.hpp"
#include "../header/UI.hpp"
Game::Game() 
    : Mario(
        Character::Builder()
        .setIdleFrames(0,0)
        .setWalkFrames(1,3)
        .setJumpFrames(5,5)
        .setFallFrames(5,5)
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



