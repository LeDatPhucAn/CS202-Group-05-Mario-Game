#pragma once
#include "Scene.hpp"
#include "Button.hpp"
#include "UI.hpp"
#include "KeyBindingManager.hpp"

class Tutorial : public Scene
{
private:
    Texture2D backgroundTexture;
    Texture2D tutorialBoard;
    Texture2D buttonTexture;
    Texture2D buttonHoverTexture;
    
    std::vector<TextBox*> buttons;
    int selectedButton = 0;
    int selectedRow = 0;
    int selectedColumn = 0; // 0 for Mario, 1 for Luigi
    bool isEditing = false;
    int editingAction = -1;
    
    // Key swap notification
    bool showSwapMessage = false;
    float swapMessageTimer = 0.0f;
    std::string swapMessage = "";
    

    std::vector<std::pair<Action, std::string>> actionList;

    void initializeActionList();
    void drawKeyBindingTable();
    void drawInstructions();
    void updateKeyBinding(int actionIndex, int player, int newKey);
    std::string getKeyName(int keyCode);
    int getCurrentKeyForAction(Action action, int player);

public:
    Tutorial(SceneManager* _manager);
    ~Tutorial();
    
    void updateScene() override;
    void displayScene() override;
};
