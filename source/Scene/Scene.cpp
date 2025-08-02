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
        {
            scenes.pop();
        }
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
    if (scenes.empty())
        return;

    // Find the base scene (Game or Menu) and collect overlay scenes
    std::vector<Scene *> overlayScenes;
    std::stack<Scene *> tempStack;
    Scene *baseScene = nullptr;

    // Pop scenes until we find a base scene (not Pause or Settings)
    while (!scenes.empty())
    {
        Scene *currentScene = scenes.top();
        scenes.pop();
        tempStack.push(currentScene);

        // Check if current scene is an overlay scene
        if (dynamic_cast<Pause *>(currentScene) || dynamic_cast<Settings *>(currentScene))
        {
            overlayScenes.push_back(currentScene);
        }
        else
        {
            // This is our base scene
            baseScene = currentScene;
            break;
        }
    }

    // Restore all scenes back to the main stack
    while (!tempStack.empty())
    {
        scenes.push(tempStack.top());
        tempStack.pop();
    }

    // Render the base scene first with camera applied
    if (baseScene)
    {
        // Apply camera if it's a Game scene
        Game *gameScene = dynamic_cast<Game *>(baseScene);
        if (gameScene)
        {
            BeginMode2D(gameScene->getCamera());
            baseScene->displayScene();
            EndMode2D();
        }
        else
        {
            // For other base scenes (like Menu), render normally
            baseScene->displayScene();
        }
    }

    // Render overlay scenes in the correct order (bottom to top)
    // These are rendered WITHOUT camera transformation (UI overlays)
    for (int i = overlayScenes.size() - 1; i >= 0; i--)
    {
        overlayScenes[i]->displayScene();
    }

    // If no overlay scenes were found, render normally
    if (overlayScenes.empty() && !baseScene)
    {
        Scene *currentScene = scenes.top();
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