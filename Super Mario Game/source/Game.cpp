#include "../header/Game.hpp"
#include "../header/UI.hpp"
Game::Game() 
    : Mario(
        Character::Builder()
        .setSprite(
            { {13, 13}, {16, 14}, {18, 18}, {18, 18} },
            UI::jsonMap["Mario2D"], UI::textureMap["Mario2D"]
            )

        .setMovement(6, { 100, 100 }, { 0, 0 }, {0,0})   // speed = 6
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
