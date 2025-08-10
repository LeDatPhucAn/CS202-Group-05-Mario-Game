#pragma once
#include "Structs.hpp"
class GameInfo {
    
private:
    static GameInfo* instance;
    
    // Score and game stats
    int score;
    int coins;
    int lives;
    
    // PreGame settings
    Mode mode;
    bool chooseMario; // for single mode: true=Mario, false=Luigi
    Difficulty difficulty;

    GameInfo() : score(0), coins(0), lives(5), mode(Mode::DUAL), chooseMario(true), difficulty(Difficulty::EASY) {}
    
public:
    GameInfo(const GameInfo&) = delete;
    GameInfo& operator=(const GameInfo&) = delete;
    
    static GameInfo* getInstance();
    
    // Score methods
    void addScore(int points);
    void addCoin();
    void setLives(int newLives);
    int getScore();
    int getCoins();
    int getLives();
    void reset();
    void resetGameOnly();
    
    // PreGame settings methods
    void setMode(Mode m) { mode = m; }
    void setCharacter(bool mario) { chooseMario = mario; }
    void setDifficulty(Difficulty d) 
    { 
        difficulty = d;
        // Set lives based on difficulty immediately
        if(d == Difficulty::EASY) lives = 5;
        else if(d == Difficulty::HARD) lives = 3;
        else if(d == Difficulty::HARDCORE) lives = 1;
    }
    
    Mode getMode() const { return mode; }
    bool isMario() const { return chooseMario; }
    Difficulty getDifficulty() const { return difficulty; }
    bool isDualMode() const { return mode == Mode::DUAL; }
    
    static void destroyInstance();
};