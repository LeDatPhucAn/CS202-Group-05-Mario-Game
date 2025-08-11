#pragma once
#include "Character.hpp"
#include "Player.hpp"
#include "MyMap.hpp"
#include "Scene.hpp"
#include "raylib-tileson.h"
#include "MovingObject.hpp"
#include "MovingObjectState.hpp"
#include "Goomba.hpp"
#include "Koopa.hpp"
#include "PiranhaPlant.hpp"
#include "Lakitu.hpp"
#include "Spiny.hpp"
#include "BulletBill.hpp"
#include "HammerBro.hpp"
#include "ContactListener.hpp"
#include "Particle.hpp"
#include <unordered_set>
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
    Texture2D HUDCoin;
    Texture2D HUDScore;

    float gameTime = 0.0f;
    float maxTime = 300.0f;
    friend class Controls;
protected:

    PlayerMario *mario = nullptr;
    PlayerLuigi *luigi = nullptr;
    Spawner *spawner = nullptr;

    string current_Map;
    MyMap curMap;

    // for box2d integration
    ContactListener *contactListener = nullptr;
    float prePosX = 0;

private:
public:
    static vector<GameObject *> gameObjects;
    static b2World *world;
    unordered_set<GameObject *> deleteLater;
    static vector<Particle> particles;
    // initialize
    Game(SceneManager *_mag);

    void init();
    static void addGameObject(GameObject *gameObject);
    void restartGame();
    void removeGameObject();
    // update
    void updateScene();

    // display
    void displayScene();
    void drawHUD();
    ~Game();
};
