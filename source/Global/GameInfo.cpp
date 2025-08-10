#include "GameInfo.hpp"

GameInfo* GameInfo::getInstance() {
    if (instance == nullptr) {
        instance = new GameInfo();
    }
    return instance;
}

void GameInfo::addScore(int points) {
    score += points;
}

void GameInfo::addCoin() {
    coins++;
}

void GameInfo::setLives(int newLives) {
    lives = newLives;
}

int GameInfo::getScore() {
    return score;
}

int GameInfo::getCoins(){
    return coins;
}

int GameInfo::getLives() {
    return lives;
}

void GameInfo::reset(){
    score=0;
    coins=0;
    //set lives based on difficulty
    switch(difficulty) {
        case Difficulty::EASY:
            lives = 5;
            break;
        case Difficulty::HARD:
            lives = 3;
            break;
        case Difficulty::HARDCORE:
            lives = 1;
            break;
        default:
            lives = 5;
            break;
    }
}

void GameInfo::resetGameOnly(){
    coins=0;
    // Keep PreGame settings intact during game reset
}

void GameInfo::destroyInstance(){
    delete instance;
    instance = nullptr;
}

GameInfo* GameInfo::instance = nullptr;


