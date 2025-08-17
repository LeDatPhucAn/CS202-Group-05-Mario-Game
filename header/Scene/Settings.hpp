#pragma once
#include "Scene.hpp"
#include "Button.hpp"

class Settings : public Scene {
private:
    std::vector<TextBox*> buttons;
    int selectedButton = 0;
    Texture2D buttonTexture;
    Texture2D buttonHoverTexture;
    Texture2D settingsBoard;

    
    // Settings values
    float musicVolume = 0.5f;
    float sfxVolume = 0.5f;
    // Removed fullscreen toggle
    
public:
    Settings(SceneManager* _manager);
    ~Settings();
    void updateScene();
    void displayScene();
};