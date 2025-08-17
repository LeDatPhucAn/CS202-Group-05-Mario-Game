#pragma once
#include "Scene.hpp"
#include "Button.hpp"

class GameOver: public Scene {
    private:
        std::vector<TextBox *> buttons;
        int selectedButton = 0;
        Texture2D backgroundTexture;
        Texture2D buttonTexture;
        Texture2D buttonHoverTexture;

    public:
        GameOver(SceneManager* _manager);
        ~GameOver();
        void updateScene() override;
        void displayScene() override;
};