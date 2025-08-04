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
class DrawDebug;
class Game : public Scene
{
private:
    void updateCharacters();
    void updateMap();
    DrawDebug *drawDebug = nullptr;
    bool showDebugDraw = false;
    Texture2D HUDLives;
    Texture2D HUDTime;

    //For display
    int lives = 3;
    float gameTime = 0.0f;
    float maxTime = 400.0f; 

protected:
    Mario *mario = nullptr;

    Spawner *spawner = nullptr;
    // Character* MarioPointer = nullptr;
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
    void removeGameObject();
    // update
    void updateScene();

    // display
    void displayScene();
    void drawHUD();
    ~Game();
};
