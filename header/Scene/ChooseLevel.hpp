#pragma once
#include "Scene.hpp"
#include "Player.hpp"
#include "raylib.h"
#include <vector>

struct LevelPortal {
    Vector2 position;
    int levelNumber;
    Rectangle bounds;
    Texture2D texture;
    std::string levelName;
};

class ChooseLevel : public Scene {
private:
    PlayerMario* mario;
    b2World* world;
    Camera2D cam; // Use cam like Game.cpp, not camera
    
    std::vector<LevelPortal> levelPortals;
    Texture2D backgroundTexture;
    vector<Texture2D> LevelTextures;
    
    int selectedLevel;
    bool levelSelected;
    float interactionTimer;
    float prePosX; 
    
    // UI elements
    std::string instructionText;
    bool showLevelInfo;
    int hoveredLevel;
    
    void initializePortals();
    void checkPortalCollisions();
    void drawUI();
    void drawPortals();
    void drawPlayer();
    
public:
    ChooseLevel(SceneManager* _manager);
    ~ChooseLevel();
    
    void updateScene() override;
    void displayScene() override;
};