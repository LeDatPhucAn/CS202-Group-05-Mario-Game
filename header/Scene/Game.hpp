#pragma once
#include "Character.hpp"
#include "Mario.hpp"
#include "MyMap.hpp"
#include "Scene.hpp"
#include "raylib-tileson.h"
#include "Enemy.hpp"
#include "EnemyState.hpp"
#include "Goomba.hpp"
#include "Koopa.hpp"
#include "PiranhaPlant.hpp"
#include "Lakitu.hpp"
#include "Spiny.hpp"
#include "ContactListener.hpp"
#include "Particle.hpp"
#include "Items.hpp"
class Game : public Scene
{
private:
    void updateCharacters();
    void updateMap();

protected:
    Mario Mario;
    Goomba Goomba;
    Koopa Koopa;
    PiranhaPlant PiranhaPlant;
    Lakitu Lakitu;

    // Character* MarioPointer = nullptr;
    std::unordered_map<std::string, std::string> mapPaths;
    string current_Map;
    MyMap curMap;

    // for box2d integration
    ContactListener *contactListener = nullptr;

public:
    static vector<Enemy *> enemies;

    static b2World *world;

    static vector<Particle> particles;

	static vector<Items*> items;
    // initialize
    Game(SceneManager *_mag);

    void init();
    static void addEnemy(Enemy *enemy);
    static void removeEnemy(Enemy *enemy);

    // update
    void updateScene();
    void updateSceneInCamera(Camera2D cam) {};

    // display
    void displayScene();
    ~Game();
};
