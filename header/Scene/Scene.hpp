#pragma once
#include "UI.hpp"
typedef enum sceneType { 
	MENU = 0,
	GAME
} sceneType;

class Scene {
public:
	sceneType CurrentScene;
	
	// intitialize
	virtual void init() = 0;
	
	// update
	virtual void updateScene() = 0;
	virtual void updateSceneInCamera(Camera2D cam) = 0;

	// display
	virtual void displayScene() = 0;
	virtual void displaySceneInCamera() = 0;
    
	
	virtual ~Scene() = default;
};
