#include "../header/Game.hpp"
#include "../header/UI.hpp"
Game::Game() 
    : Mario(
        Character::Builder()
        .setSprite({ {6, 6}, {7, 9} }, UI::jsonMap["Mario"], UI::textureMap["Mario"])
        .setMovement(2, { 100, 100 }, { 0, 0 }, {0,0})   // speed = 2
        .build()
    )
{
    init();
}

void Game::updateScene() {
    Mario.update();
}
void Game::displaySceneInCamera() {
    Mario.draw();
}
void Game::displayScene() {

}


void Game::init() {
    Mario.changeState(new IdleState(&Mario));
}
