#pragma once
#include <stack>
#include "Scene.hpp"

using namespace std;

class Program {
private:
    SceneManager mySceneManager;
public:
    Program();
    void update();
    void display();
    void run();
};
