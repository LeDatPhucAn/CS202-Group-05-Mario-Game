#include "Scene.hpp"
#include "Menu.hpp"
#include "Game.hpp"
#include "Settings.hpp"
#include "Pause.hpp"
#include "GameOver.hpp"
#include "ChooseLevel.hpp"
#include <iostream>
#include <stack>
#include "SoundController.hpp"
using namespace std;
Scene::Scene(SceneManager *_manager) : manager(_manager) {}

void SceneManager::init()
{
    SoundController::getInstance().playSceneMusic(sceneType::MENU);
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
        SoundController::getInstance().playSceneMusic(_nextScene);
        if (_nextScene == sceneType::PAUSE || _nextScene == sceneType::SETTING || _nextScene == sceneType::GAMEOVER || _nextScene == sceneType::CHOOSE_LEVEL)
            numBackToBaseScene++;
        scenes.push_back(SceneFactory::create(_nextScene, this));
    }
}

void SceneManager::goBack()
{
    needBack = 1;
}

void SceneManager::goBackOfBaseScene()
{
    needBack = 1 + numBackToBaseScene;
}

void SceneManager::update()
{
    // cout << "Scene.size()" << scenes.size() << " " << numBackToBaseScene << endl;
    if (needBack > 0)
    {
        while (!scenes.empty() && needBack > 0)
        {
            delete scenes.back();
            scenes.pop_back();
            needBack--;

            if (numBackToBaseScene > 0)
                numBackToBaseScene--;
        }
        cout << "Delete Scene" << endl;
    }

    if (!scenes.empty())
    {
        SoundController::getInstance().updateSceneMusic();
        scenes.back()->updateScene();
    }
}

void SceneManager::display()
{
    for (int i = scenes.size() - 1 - numBackToBaseScene; i < scenes.size(); i++)
    {
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
}

Scene *SceneFactory::create(sceneType newScene, SceneManager *mag)
{
    cout << "create\n";
    if (newScene == sceneType::NONE)
        return nullptr;
    if (newScene == sceneType::MENU)
        return new Menu(mag);
    if (newScene == sceneType::SETTING)
        return new Settings(mag);
    if (newScene == sceneType::PAUSE)
        return new Pause(mag);
    if (newScene == sceneType::GAMEOVER)
        return new GameOver(mag);
    if (newScene == sceneType::CHOOSE_LEVEL)
        return new ChooseLevel(mag);
    
    if (newScene == sceneType::GAME)
        return new Game(mag);

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