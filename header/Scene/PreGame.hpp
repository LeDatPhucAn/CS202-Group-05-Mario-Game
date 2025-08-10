
#pragma once
#include "Scene.hpp"
#include "Button.hpp"
class PreGame : public Scene {
public:
	enum class Mode { SINGLE, DUAL };
	enum class Difficulty { EASY, HARD };
private:
	Mode mode = Mode::DUAL;        // single or dual
	bool chooseMario = true;       // if single: true -> Mario, false -> Luigi
	Difficulty difficulty = Difficulty::EASY;
	int selectionIndex = 0;        // which row is highlighted
	bool launching = false;        // trigger start
public:
	PreGame(SceneManager* m);
	void updateScene() override;
	void displayScene() override;
};
