#include "Score.hpp"

Score* Score::getInstance() {
    if (instance == nullptr) {
        instance = new Score();
    }
    return instance;
}

void Score::addScore(int points) {
    score += points;
}

void Score::addCoin() {
    coins++;
}

void Score::setLives(int newLives) {
    lives = newLives;
}

int Score::getScore() {
    return score;
}

int Score::getCoins(){
    return coins;
}

int Score::getLives() {
    return lives;
}

void Score::reset(){
    score=0;
    coins=0;
    lives=5;
}

void Score::resetGameOnly(){
    coins=0;
}

void Score::destroyInstance(){
    delete instance;
    instance = nullptr;
}

Score* Score::instance = nullptr;


