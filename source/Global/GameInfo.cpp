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
    lives=5;
    // Reset PreGame settings to defaults
    mode = Mode::DUAL;
    chooseMario = true;
    difficulty = Difficulty::EASY;
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


