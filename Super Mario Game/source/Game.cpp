#include "../header/Game.hpp"
#include "../header/UI.hpp"
Game::Game()
    : Mario(
        Character::Builder()
        .setIdleFrames(0, 0)
        .setWalkFrames(1, 3)
        .setJumpFrames(5, 5)
        .setFallFrames(5, 5)
        .setSkidFrames(4, 4)
        .setRunFrames(1, 3)
        .setJsonAndTexture("Mario2D")
        .setPos({ 100,0 })
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
    Rectangle rect = { 0,0,textureMap["1-1"].width, textureMap["1-1"].height };
    DrawTexturePro(textureMap["1-1"], rect, rect, { 0,0 }, 0, WHITE);
    Mario.draw();
}
void Game::displayScene() {
    
}



