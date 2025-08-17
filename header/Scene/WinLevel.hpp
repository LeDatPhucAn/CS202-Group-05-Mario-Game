#pragma once
#include "Scene.hpp"
#include <string>

class WinLevel : public Scene {
public:
    WinLevel(SceneManager* manager);
    ~WinLevel() override;
    void updateScene() override;
    void displayScene() override;
private:
    bool anyKeyPressed;
    string mapName;

    // GIF animation members
    Image gifImage;
    Texture2D gifTexture;
    int frameCount;
    int currentFrame;
    float frameTime;
    float frameDuration;
    bool gifLoaded;
};
