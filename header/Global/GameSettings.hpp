#pragma once

class GameSettings {
public:
    enum class Mode { SINGLE, DUAL };
    enum class Difficulty { EASY, HARD };
    
private:
    static GameSettings* instance;
    Mode mode = Mode::DUAL;
    bool chooseMario = true; // for single mode
    Difficulty difficulty = Difficulty::EASY;
    
    GameSettings() = default;
    
public:
    static GameSettings* getInstance();
    
    void setMode(Mode m) { mode = m; }
    void setCharacter(bool mario) { chooseMario = mario; }
    void setDifficulty(Difficulty d) { difficulty = d; }
    
    Mode getMode() const { return mode; }
    bool isMario() const { return chooseMario; }
    Difficulty getDifficulty() const { return difficulty; }
    bool isDualMode() const { return mode == Mode::DUAL; }
    
    static void destroyInstance();
};
