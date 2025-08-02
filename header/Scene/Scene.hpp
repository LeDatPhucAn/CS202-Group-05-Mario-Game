#pragma once
#include <stack>
#include "raylib.h"
using namespace std;
enum class sceneType
{
	NONE,
	MENU,
	GAME,
	CHOOSE_LEVEL,
	PAUSE,
	SETTING
};

class SceneManager;
class Program;
class Scene
{
protected:
	Camera2D cam = {{0, 0}, {0, 0}, 0.0f, 1.0f};

public:
	SceneManager *manager = nullptr;
	Scene() = default;
	Scene(SceneManager *_manager);

	// update_ Để đổi Scene thì gọi manager.changeScene(sceneType::ABC)
	virtual void updateScene() = 0;

	// display
	virtual void displayScene() = 0;

	virtual ~Scene() = default;

	virtual Camera2D getCamera();
};

class SceneManager
{
public:
	stack<Scene *> scenes;
	sceneType nextScene = sceneType::NONE;
	Program *pro;
	bool needBack = 0;

	void changeScene(sceneType nextScene);
	void goBack();

	void init();
	void update();
	void display();

	~SceneManager();

	Camera2D getCamera();
};

class SceneFactory
{
public:
	static Scene *create(sceneType newScene, SceneManager *mag);
};
