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
#include "Projectile.hpp"
class Game : public Scene
{
private:
    void updateCharacters();
    void updateMap();

protected:
    Mario *mario = nullptr;
    Goomba *goomba = nullptr;
    Koopa *koopa = nullptr;
    PiranhaPlant *piranhaPlant = nullptr;
    Lakitu *lakitu = nullptr;

    // Character* MarioPointer = nullptr;
    std::unordered_map<std::string, std::string> mapPaths;
    string current_Map;
    MyMap curMap;

    // for box2d integration
    ContactListener *contactListener = nullptr;

    float prePosX = 0;

public:
    static vector<GameObject *> gameObjects;
    static b2World *world;

    static vector<Particle> particles;
    // initialize
    Game(SceneManager *_mag);

    void init();
    static void addGameObject(GameObject *gameObject);
    static void removeGameObject(GameObject *gameObject);
    // update
    void updateScene();
    void updateSceneInCamera(Camera2D cam) {};

    // display
    void displayScene();
    ~Game();
};
