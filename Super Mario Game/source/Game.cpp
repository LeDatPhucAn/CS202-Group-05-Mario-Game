#include "../header/Game.hpp"
Game::Game() 
    : Mario(14, 5, 4, { 0, 0 }, { 100, 200 }, LoadTexture("assets/Sprites/mario.png"), { 1, 3 }){}


void Game::updateScene() {
    Mario.update();
}

void Game::displayScene() {
    //Mario.draw();
}

void Game::displaySceneInCamera() {
    Mario.draw();
}
void Game::init() {
    initButtons();
}

void Game::initButtons() {

}