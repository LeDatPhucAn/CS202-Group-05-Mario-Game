#pragma once
#include "UI.hpp"
//#include "AnimationManager.h"
typedef enum Scenes { 
	MENU = 0,
	GAME
} Scene;

class SceneManager : public UI{
public:
	Scene CurrentScene;
	//AnimationManager animManager;
	virtual void replayOperation() {};
	virtual void init() = 0;
	virtual void initButtons() = 0;
	virtual void resetAnimations() = 0;
	virtual void updateButtonPositions() = 0;
	virtual void updateScene() = 0;
	virtual void updateSceneInCamera(Camera2D cam) = 0;
	virtual void displayScene() = 0;
	virtual void displaySceneInCamera() = 0;
    virtual ~SceneManager() = default;
};
