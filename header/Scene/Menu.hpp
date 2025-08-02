#pragma once
#include "Scene.hpp"
#include "Button.hpp"

#pragma once
#include "Scene.hpp"
#include "Button.hpp"

class Program;
class Menu : public Scene
{
private:
    std::vector<TextBox *> buttons;
    int selectedButton = 0;
    Texture2D backgroundTexture;
    Texture2D titleTexture;
    Texture2D buttonTexture;
    Texture2D buttonHoverTexture;
    vector<Texture2D> buttonTextures;
    Texture2D selectionIconTexture;

public:
    Menu(SceneManager *_manager);
    ~Menu();
    void updateScene();
    void displayScene();
};
