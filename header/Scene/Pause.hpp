#pragma once
#include "Scene.hpp"
#include "Button.hpp"

class Pause: public Scene {
    private:
        std::vector<TextBox *> buttons;
        int selectedButton = 0;
        Texture2D pausePanel;
        Texture2D buttonTexture;
        Texture2D buttonHoverTexture;

    public:
        Pause(SceneManager* _manager);
        ~Pause();
        void updateScene() override;
        void displayScene() override;
};