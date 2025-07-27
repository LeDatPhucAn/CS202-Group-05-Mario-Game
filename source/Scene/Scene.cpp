#include "Scene.hpp"
#include "Menu.hpp"
#include "Game.hpp"
#include "Settings.hpp"
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