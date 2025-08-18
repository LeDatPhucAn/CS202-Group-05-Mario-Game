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

    Texture2D backgroundTexture;

    // GIF animation members for level completion
    Image gifImage;
    Texture2D gifTexture;
    int frameCount;
    int currentFrame;
    float frameTime;
    float frameDuration;
    bool gifLoaded;

    // GIF animation members for final victory (Map 3)
    Image victoryGifImage;
    Texture2D victoryGifTexture;
    int victoryFrameCount;
    int victoryCurrentFrame;
    float victoryFrameTime;
    bool victoryGifLoaded;
};
