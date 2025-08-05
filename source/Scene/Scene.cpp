#include "Scene.hpp"
#include "Menu.hpp"
#include "Game.hpp"
#include "Settings.hpp"
#include "Pause.hpp"
#include "GameOver.hpp"
#include <iostream>
#include <stack>
using namespace std;
Scene::Scene(SceneManager *_manager) : manager(_manager) {}

void SceneManager::init()
{
    scenes.push_back(SceneFactory::create(sceneType::MENU, this));
}
SceneManager::~SceneManager()
{
    while (!scenes.empty())
    {
        delete scenes.back();
        scenes.pop_back();
    }
}

void SceneManager::changeScene(sceneType _nextScene)
{
    if (_nextScene != sceneType::NONE)
    {
        if(_nextScene == sceneType::PAUSE || _nextScene == sceneType::SETTING || _nextScene == sceneType::GAMEOVER)
            numBackToBaseScene++;
        scenes.push_back(SceneFactory::create(_nextScene, this));
    }
}

void SceneManager::goBack() {
    needBack = 1;
}

void SceneManager::goBackOfBaseScene() {
    needBack = 1 + numBackToBaseScene;
}

void SceneManager::update()
{
    // cout << "Scene.size()" << scenes.size() << " " << numBackToBaseScene << endl;
    if (needBack > 0)
    {
        while(!scenes.empty() &&  needBack > 0)
        {
            delete scenes.back();
            scenes.pop_back();
            needBack--;

            if(numBackToBaseScene > 0)
                numBackToBaseScene--;
        }
        cout << "Delete Scene" << endl;
    }

    if(!scenes.empty())
        scenes.back()->updateScene();

}

void SceneManager::display()
{
    for(int i = scenes.size()-1 - numBackToBaseScene; i < scenes.size(); i++) {
        Scene *currentScene = scenes[i];
        Game *gameScene = dynamic_cast<Game *>(currentScene);
        if (gameScene)
        {
            BeginMode2D(gameScene->getCamera());
            currentScene->displayScene();
            EndMode2D();
        }
        else
        {
            currentScene->displayScene();
        }
    }

    // if (scenes.empty())
    //     return;

    // // Find the base scene (Game or Menu) and collect overlay scenes
    // std::vector<Scene *> overlayScenes;
    // std::stack<Scene *> tempStack;
    // Scene *baseScene = nullptr;

    // // Pop scenes until we find a base scene (not Pause or Settings)
    // while (!scenes.empty())
    // {
    //     Scene *currentScene = scenes.back();
    //     scenes.pop_back();
    //     tempStack.push(currentScene);

    //     // Check if current scene is an overlay scene
    //     if (dynamic_cast<Pause *>(currentScene) || dynamic_cast<Settings *>(currentScene))
    //     {
    //         overlayScenes.push_back(currentScene);
    //     }
    //     else
    //     {
    //         // This is our base scene
    //         baseScene = currentScene;
    //         break;
    //     }
    // }

    // // Restore all scenes back to the main stack
    // while (!tempStack.empty())
    // {
    //     scenes.push_back(tempStack.top());
    //     tempStack.pop();
    // }

    // // Render the base scene first with camera applied
    // if (baseScene)
    // {
    //     // Apply camera if it's a Game scene
    //     Game *gameScene = dynamic_cast<Game *>(baseScene);
    //     if (gameScene)
    //     {
    //         BeginMode2D(gameScene->getCamera());
    //         baseScene->displayScene();
    //         EndMode2D();
    //     }
    //     else
    //     {
    //         // For other base scenes (like Menu), render normally
    //         baseScene->displayScene();
    //     }
    // }

    // // Render overlay scenes in the correct order (bottom to top)
    // // These are rendered WITHOUT camera transformation (UI overlays)
    // for (int i = overlayScenes.size() - 1; i >= 0; i--)
    // {
    //     overlayScenes[i]->displayScene();
    // }

    // // If no overlay scenes were found, render normally
    // if (overlayScenes.empty() && !baseScene)
    // {
    //     Scene *currentScene = scenes.back();
    //     Game *gameScene = dynamic_cast<Game *>(currentScene);
    //     if (gameScene)
    //     {
    //         BeginMode2D(gameScene->getCamera());
    //         currentScene->displayScene();
    //         EndMode2D();
    //     }
    //     else
    //     {
    //         currentScene->displayScene();
    //     }
    // }
}

Scene *SceneFactory::create(sceneType newScene, SceneManager *mag)
{
    cout << "create\n";
    if (newScene == sceneType::NONE)
        return nullptr;
    if (newScene == sceneType::MENU)
        return new Menu(mag);
    if (newScene == sceneType::GAME)
        return new Game(mag);
    if (newScene == sceneType::SETTING)
        return new Settings(mag);
    if (newScene == sceneType::PAUSE)
        return new Pause(mag);
    if (newScene == sceneType::GAMEOVER) 
        return new GameOver(mag);
    
    

    // if(newScene == CHOOSE_LEVEL)
    // 	retunr new ChooseLevel(mag); ....

    return nullptr;
}

Camera2D Scene::getCamera()
{
    return cam;
}

Camera2D SceneManager::getCamera()
{
    return scenes.back()->getCamera();
}