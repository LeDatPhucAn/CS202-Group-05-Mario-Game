#pragma once
#include "Character.hpp"
#include "Player.hpp"
#include "MyMap.hpp"
#include "Scene.hpp"
#include "raylib-tileson.h"
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


    //tạo hiệu ứng chuyển cảnh khi bắt đầu game
    RenderTexture2D transitionTexture;
    bool isTransitioning = true;
    float transitionTimer = 0.0f;
    float transitionDuration = 1.5f;
    float circleRadius = 0.0f;
    Vector2 circleCenter;
    
    void updateTransition();
    void drawTransition();
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
