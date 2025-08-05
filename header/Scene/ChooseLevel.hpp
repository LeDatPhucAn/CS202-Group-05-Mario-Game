#pragma once
#include "Scene.hpp"
#include "Mario.hpp"
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
    Mario* mario;
    b2World* world;
    Camera2D camera;
    
    std::vector<LevelPortal> levelPortals;
    Texture2D backgroundTexture;
    Texture2D groundTexture;
    Texture2D portalTexture;
    
    int selectedLevel;
    bool levelSelected;
    float interactionTimer;
    
    // UI elements
    std::string instructionText;
    bool showLevelInfo;
    int hoveredLevel;
    
    void initializePortals();
    void updateCamera();
    void checkPortalCollisions();
    void drawUI();
    void drawPortals();
    void drawMario();
    
public:
    ChooseLevel(SceneManager* _manager);
    ~ChooseLevel();
    
    void updateScene() override;
    void displayScene() override;
};