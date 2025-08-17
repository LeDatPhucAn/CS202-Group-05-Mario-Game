#include "GameInfo.hpp"
#include <fstream>
#include <filesystem>
using namespace std;

GameInfo* GameInfo::getInstance() {
    if (instance == nullptr) {
        instance = new GameInfo();
    }
    return instance;
}

void GameInfo::addScore(int points) {
    score += points;
    updateHighScore();
}

void GameInfo::addCoin() {
    coins += 1;
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
}

void GameInfo::destroyInstance(){
    delete instance;
    instance = nullptr;
}

GameInfo* GameInfo::instance = nullptr;

void GameInfo::loadHighScoreFromFile() {
    const char* path = "assets/highscore.txt";
    ifstream in(path);
    if (in.is_open()) {
        int hs = 0;
        if (in >> hs) {
            highScore = hs;
        }
    }
}

void GameInfo::saveHighScoreToFile() {
    const char* path = "assets/highscore.txt";
    ofstream out(path, ios::trunc);
    if (out.is_open()) {
        out << highScore;
    }
}

void GameInfo::updateHighScore() {
    if (score > highScore) {
        highScore = score;
        saveHighScoreToFile();
    }
}


