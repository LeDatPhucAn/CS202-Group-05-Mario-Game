#include "Scene.hpp"
#include "Menu.hpp"
#include "Game.hpp"
#include "Settings.hpp"
#include "Pause.hpp"
#include <iostream>
using namespace std;
Scene::Scene(SceneManager *_manager) : manager(_manager) {}

void SceneManager::init()
{
    scenes.push(SceneFactory::create(sceneType::MENU, this));
}
SceneManager::~SceneManager()
{
    while (!scenes.empty())
    {
        delete scenes.top();
        scenes.pop();
    }
}

void SceneManager::changeScene(sceneType nextScene)
{
    if (needBack)
    {
        if (!scenes.empty())
            scenes.pop();
    }
    if (nextScene != sceneType::NONE)
    {
        scenes.push(SceneFactory::create(nextScene, this));
        nextScene = sceneType::NONE;
    }
}

void SceneManager::goBack()
{
    needBack = true;
}

void SceneManager::update()
{
    if (needBack)
    {
        if (!scenes.empty())
            scenes.pop();
        needBack = false;
        cout << "SMag::update::goBack";
        return;
    }
    scenes.top()->updateScene();
}

void SceneManager::display()
{

    if (scenes.size() >= 2) {
        // Create a temporary stack to access the previous scene
        std::stack<Scene*> tempStack;
        Scene* currentScene = scenes.top();
        
        // Check if current scene is pause
        if (dynamic_cast<Pause*>(currentScene)) {
            tempStack.push(currentScene);
            scenes.pop();
            
            // Get the previous scene (should be Game)
            Scene* previousScene = scenes.top();
            
            // Render the previous scene first
            previousScene->displayScene();
            
            // Restore the pause scene to the stack
            scenes.push(tempStack.top());
            tempStack.pop();
            
            // Now render the pause overlay on top
            currentScene->displayScene();
            return;
        }
    }
    // Normal scene rendering
    scenes.top()->displayScene();
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
    return scenes.top()->getCamera();
}