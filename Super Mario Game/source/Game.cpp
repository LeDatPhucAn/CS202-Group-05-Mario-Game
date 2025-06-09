#include "../header/Game.hpp"
#include "../header/UI.hpp"
Game::Game()
    : Mario(
        Character::Builder()
        .setFrames(IDLE, 0, 0)
        .setFrames(WALK, 1, 3)
        .setFrames(JUMP, 5, 5)
        .setFrames(FALL, 5, 5)
        .setFrames(SKID, 4, 4)
        .setFrames(RUN, 1, 3)
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



