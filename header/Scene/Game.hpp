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
class Spawner;
class Game : public Scene
{
private:
    void updateCharacters();
    void updateMap();

protected:
    Mario *mario = nullptr;

    Spawner *spawner = nullptr;
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
    vector<GameObject *> deleteLater;
    static vector<Particle> particles;
    // initialize
    Game(SceneManager *_mag);

    void init();
    static void addGameObject(GameObject *gameObject);
    static void removeGameObject(GameObject *gameObject);
    // update
    void updateScene();

    // display
    void displayScene();
    ~Game();
};
