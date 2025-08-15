#include "Game.hpp"
#include "UI.hpp"
#include "PlayerState.hpp"
#include "Pause.hpp"
#include <chrono>
#include <thread>
#include "BlockState.hpp"
#include "Spawner.hpp"
#include "DrawDebug.hpp"
#include "GameInfo.hpp"
#include "MovingObject.hpp"
#include "MovingObjectState.hpp"
#include "Goomba.hpp"
#include "Koopa.hpp"
#include "PiranhaPlant.hpp"
#include "Lakitu.hpp"
#include "Spiny.hpp"
#include "BulletBill.hpp"
#include "HammerBro.hpp"

vector<Particle> Game::particles = {};
b2World *Game::world = nullptr;
vector<GameObject *> Game::gameObjects = {};

Game::Game(SceneManager *_mag)
{
    manager = _mag;

    world = new b2World({0, fallGravity});
    contactListener = new ContactListener();
    world->SetContactListener(contactListener);

    init();
}

void Game::init()
{   
    HUDLives = LoadTexture("assets/Backgrounds/HUD/lives.png");
    if (HUDLives.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load HUD Lives texture");
    }

    HUDTime = LoadTexture("assets/Backgrounds/HUD/time.png");
    if (HUDTime.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load HUD Time texture");
    }

    HUDCoin = LoadTexture("assets/Backgrounds/HUD/coin.png");
    if (HUDCoin.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load HUD Coin texture");
    }

    HUDScore = LoadTexture("assets/Backgrounds/HUD/score.png");
    if (HUDScore.id == 0)
    {
        TraceLog(LOG_WARNING, "Failed to load HUD Score texture");
    }

    transitionTexture = LoadRenderTexture(UI::screenWidth, UI::screenHeight);
    isTransitioning = true;
    transitionTimer = 0.0f;
    circleRadius = 0.0f;
    circleCenter = {UI::screenWidth / 2.0f, UI::screenHeight / 2.0f};


    // Get game settings
    GameInfo* gameInfo = GameInfo::getInstance();
    
    // Instantiate main characters based on mode
    if(gameInfo->isDualMode()) {
        mario = new PlayerMario();
        luigi = new PlayerLuigi();
        mario->setPosition(curMap.StartingPoint);
        luigi->setPosition({curMap.StartingPoint.x + 32, curMap.StartingPoint.y});
    } else {
        // Single player mode - spawn only chosen character
        if(gameInfo->isMario()) {
            mario = new PlayerMario();
            mario->setPosition(curMap.StartingPoint);
            luigi = nullptr;
        } else {
            luigi = new PlayerLuigi();
            luigi->setPosition(curMap.StartingPoint); // Luigi takes Mario's default position in single mode
            mario = nullptr;
        }
    }

    spawner = new Spawner(this);
    curMap.setSpawner(spawner);
    // Load map
    current_Map = manager->curMap;
    curMap.choose(UI::mapPaths[current_Map]);

    drawDebug = new DrawDebug();
    // Prepare map blocks (add blocks to Box2D world)
    for (auto &block : curMap.tileBlocks)
    {
        block->changeState(new BlockIdleState(block));
        block->createBody(world);
    }

    // Add enemies to game
    spawner->spawnEnemy();
    if(luigi) addGameObject(luigi);
    if(mario) addGameObject(mario);
    // Initialize camera
    cam.offset = {0, 0};
    cam.target = {0, 0};
    cam.zoom = static_cast<float>(UI::screenHeight) / (WorldHeight-16);
    cam.rotation = 0;
    prePosX = (mario) ? mario->getPosition().x : ((luigi) ? luigi->getPosition().x : 100);
    prePosXcam = GetScreenToWorld2D({0,0}, cam).x + 8;

    gameTime = 0.0f;
    GameInfo::getInstance()->reset();
    SoundController::getInstance().playSceneMusicFromStart(sceneType::GAME);
}

Game::~Game()
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i])
        {
            delete gameObjects[i];
            gameObjects[i] = nullptr;
        }
    }
    gameObjects.clear();

    delete world;
    world = nullptr;

    delete drawDebug;
    drawDebug = nullptr;

    delete contactListener;
    contactListener = nullptr;

    delete spawner;
    spawner = nullptr;

    if (HUDLives.id != 0)
    {
        UnloadTexture(HUDLives);
        HUDLives.id = 0;
    }

    if (HUDTime.id != 0)
    {
        UnloadTexture(HUDTime);
        HUDTime.id = 0;
    }

    if (HUDCoin.id != 0)
    {
        UnloadTexture(HUDCoin);
        HUDCoin.id = 0;
    }

    if (HUDScore.id != 0)
    {
        UnloadTexture(HUDScore);
        HUDScore.id = 0;
    }

    UnloadRenderTexture(transitionTexture);
}

void Game::restartGame()
{   
    isTransitioning = true;
    transitionTimer = 0.0f;
    circleRadius = 0.0f;
    circleCenter = {UI::screenWidth / 2.0f, UI::screenHeight / 2.0f};


    for (auto &obj : gameObjects)
    {
        if (obj && obj != mario && obj != luigi)
        {
            obj->needDeletion = true;
        }
    }

    SoundController::getInstance().playSceneMusicFromStart(sceneType::GAME);
    // Reset mario
    if (mario) mario->reset(curMap.StartingPoint);
    if (luigi) luigi->reset({curMap.StartingPoint.x+10, curMap.StartingPoint.y});
    // reset camera
    cam.target = {0, 0};

    prePosX = (mario) ? mario->getPosition().x : ((luigi) ? luigi->getPosition().x : 100);
    prePosXcam = GetScreenToWorld2D({0,0}, cam).x;
    // Reset game time
    gameTime = 0.0f;

    // Reset lives and coins but keep score
    GameInfo::getInstance()->resetGameOnly();

    // Respawn enemies
    spawner->spawnEnemy();
}

void Game::addGameObject(GameObject *gameObject)
{
    if (gameObject)
    {
        gameObject->createBody(world);
        gameObjects.push_back(gameObject);
    }
}

void Game::removeGameObject()
{

    // Delete non-Block
    for (GameObject *obj : deleteLater)
    {
        // a) erase from the main list
        auto it = std::find(gameObjects.begin(), gameObjects.end(), obj);
        if (it != gameObjects.end())
        {
            gameObjects.erase(it);
            if (obj->getBody())
            {
                world->DestroyBody(obj->getBody());
                obj->attachBody(nullptr);
            }
            delete obj;
            obj = nullptr;
        }
    }
    deleteLater.clear();
    // Delete Block
    auto &blocks = curMap.tileBlocks;
    vector<Block *> toDelete;

    blocks.erase(
        std::remove_if(blocks.begin(), blocks.end(), [&](Block *block)
                       {
        if (block->needDeletion) {
            toDelete.push_back(block);
            world->DestroyBody(block->getBody());
            return true; // mark for removal
        }
        return false; }),
        blocks.end());

    for (Block *&block : toDelete)
    {
        block->behavior->block = nullptr;
        delete block;
        block = nullptr;
    }
}

void Game::updateScene()
{
    // Update transition first
    updateTransition();

    //Update key bindings after pause
    static bool needsKeyUpdated = false;

    if (needsKeyUpdated)
    {
        if (mario) mario->updateKeyBindings();
        if (luigi) luigi->updateKeyBindings();
        needsKeyUpdated = false;
    }

    // Handle death state
    static float deathTimer = 3.0f;
    static bool inDeathAnimation = false;

    DeadState *deadState = nullptr;
    DeadState *deadState2 = nullptr;
    if (mario && mario->currentState)
    {
        deadState = dynamic_cast<DeadState *>(mario->currentState);
    }
    if (luigi && luigi->currentState)
    {
        deadState2 = dynamic_cast<DeadState *>(luigi->currentState);
    }
    if ((mario && deadState) || (luigi && deadState2))
    {
        if (!inDeathAnimation)
        {
            // Start death animation
            inDeathAnimation = true;
            deathTimer = 0.0f;
            cout << "Death animation started!" << endl;
        }

        // Update death timer
        deathTimer += GetFrameTime();

        // Check if animation delay has passed (2 seconds)
        if (deathTimer >= 2.0f)
        {
            int CurrentLives = GameInfo::getInstance()->getLives();
            GameInfo::getInstance()->setLives(CurrentLives - 1);
            CurrentLives = GameInfo::getInstance()->getLives();

            if (CurrentLives > 0)
            {
                restartGame();
            }
            else
            {
                restartGame();
                manager->changeScene(sceneType::GAMEOVER);
            }

            // Reset the animation state after handling death
            inDeathAnimation = false;
            deathTimer = 0.0f;
        }
    }
    else
    {
        // Reset animation state when not in dead state
        inDeathAnimation = false;
        deathTimer = 0.0f;
    }

    if (IsKeyPressed(KEY_P))
    {
        needsKeyUpdated = true;
        manager->changeScene(sceneType::PAUSE);
    }
    if (IsKeyPressed(KEY_Q))
        showDebugDraw = !showDebugDraw;

    // Update time
    gameTime += GetFrameTime();

    // Step the world
    if (world) // 60 fps
        world->Step(1.0f / 60.0f, 6, 2);

    updateCharacters();
    updateMap();

    //When Mario's in EndZone
    if(mario && CheckCollisionRecs(mario->getBounds(), curMap.EndZone) 
    || luigi && CheckCollisionRecs(luigi->getBounds(), curMap.EndZone))
    {
        //Thêm Win Scene
        manager->goBack();
        manager->curMap = curMap.nextMap;
    }
    removeGameObject();
    updateMyCamera();

}
void Game::updateMyCamera() {
    Player* aheadPlayer = nullptr;
    Player* behindPlayer = nullptr;
    if (mario && luigi) {
        if (mario->getPosition().x >= luigi->getPosition().x) {
            aheadPlayer = mario;
            behindPlayer = luigi;
        } else {
            aheadPlayer = luigi;
            behindPlayer = mario;
        }
    } 
    else if (mario) {
        aheadPlayer = behindPlayer = mario;
    } 
    else if (luigi) {
        aheadPlayer = behindPlayer = luigi;
    }

    if (!aheadPlayer) return; 

    float leftLimit = prePosXcam; 
    float deltaX = behindPlayer->getPosition().x - prePosX;
    float followPoint = cam.target.x + 0.5f * UI::screenWidth;

    float delta = (float)behindPlayer->getPosition().x - prePosX; 
    if (behindPlayer && GetWorldToScreen2D(behindPlayer->getPosition(), cam).x > 0.5 * UI::screenWidth ) 
        cam.target.x += (delta > 0.5) ? delta : 0, 
        prePosXcam += (delta > 0.5) ? delta : 0; 
    prePosX = behindPlayer->getPosition().x;

    auto clampLeft = [&](Player* p) {
        if (!p) return;
        if (p->getPosition().x < leftLimit) {
            p->setPosition({leftLimit, p->getPosition().y});
            p->getBody()->SetTransform(
                b2Vec2(leftLimit / PPM, p->getPosition().y / PPM), 0);
        }
    };
    clampLeft(aheadPlayer);
    clampLeft(behindPlayer);

    float minGap = 16.0f; // khoảng cách tối thiểu tính theo world unit
    if (behindPlayer != aheadPlayer) 
    {
        float screenRightWorld = cam.target.x +  (UI::screenWidth / cam.zoom);
        float marginRight = 8.0f;
        float maxAheadX = screenRightWorld - marginRight;
        if (aheadPlayer->getPosition().x > maxAheadX) {
            aheadPlayer->setPosition({maxAheadX, aheadPlayer->getPosition().y});
            aheadPlayer->getBody()->SetTransform(
                b2Vec2(maxAheadX / PPM, aheadPlayer->getPosition().y / PPM), 0);
        }
    }
}


void Game::updateCharacters()
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i])
        {
            MovingObject *enemy = dynamic_cast<MovingObject *>(gameObjects[i]);
            if (enemy)
            {
                Player* activePlayer = mario ? static_cast<Player*>(mario) : static_cast<Player*>(luigi);
                if (activePlayer) {
                    enemy->update(activePlayer->getPosition());
                }
            }
            else
                gameObjects[i]->update();

            if (gameObjects[i]->needDeletion)
            {
                gameObjects[i]->needDeletion = false;
                deleteLater.insert(gameObjects[i]);
            }
        }
    }

    
}
void Game::updateMap()
{
    curMap.update();

    for (auto &x : particles)
        x.update();
}

void Game::updateTransition()
{
    if (isTransitioning)
    {
        transitionTimer += GetFrameTime();
        
        // Calculate progress (0.0 to 1.0)
        float progress = transitionTimer / transitionDuration;
        
        if (progress >= 1.0f)
        {
            // Transition complete
            isTransitioning = false;
            circleRadius = 0.0f;
        }
        else
        {
            // Smooth easing function (ease-out cubic)
            float easedProgress = 1.0f - powf(1.0f - progress, 3.0f);
            
            // Calculate the maximum radius needed to cover the entire screen
            float maxRadius = sqrtf(powf(UI::screenWidth / 2.0f, 2) + powf(UI::screenHeight / 2.0f, 2)) + 50.0f;
            
            // Expand the circle radius
            circleRadius = easedProgress * maxRadius;
        }
    }
}



void Game::displayScene()
{
    for (auto *b : curMap.imageBlocks)
        b->display();
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (gameObjects[i])
        {
            gameObjects[i]->display();
        }
    }
    for (auto *b : curMap.tileBlocks)
        b->display();

    float dt = GetFrameTime();
    for (auto &x : particles)
    {
        x.display(dt);
    }
    if (showDebugDraw)
        drawDebug->DrawWorld(world);
    drawHUD();

    // Draw transition effect on top
    drawTransition();
}

#include "rlgl.h"

void Game::drawTransition()
{   

    if (isTransitioning)
    {
        // Step 1: Render the entire game scene to a texture
        RenderTexture2D sceneTexture = LoadRenderTexture(UI::screenWidth, UI::screenHeight);
        BeginTextureMode(sceneTexture);
        ClearBackground(BLANK);

        BeginMode2D(cam);

        // draw your scene elements again here:
        for (auto *b : curMap.imageBlocks) b->display();
        for (auto *obj : gameObjects) if (obj) obj->display();
        for (auto *b : curMap.tileBlocks) b->display();
        drawHUD();
        EndMode2D();
        for (auto &p : particles) p.display(GetFrameTime());

        EndTextureMode();

        // Step 2: Draw black screen
        DrawRectangle(0, 0, UI::screenWidth, UI::screenHeight, BLACK);

        // Step 3: Draw only the circle portion of the scene
        BeginScissorMode(
            circleCenter.x - circleRadius,
            circleCenter.y - circleRadius,
            circleRadius * 2,
            circleRadius * 2
        );
        DrawTextureRec(
            sceneTexture.texture,
            { 0, 0, (float)sceneTexture.texture.width, -(float)sceneTexture.texture.height },
            { 0, 0 },
            WHITE
        );
        EndScissorMode();

        // Step 4: Unload scene texture
        UnloadRenderTexture(sceneTexture);
    }
}

void Game::drawHUD()
{
    // Calculate remaining time
    int remainingTime = (int)(maxTime - gameTime);
    if (remainingTime < 0)
        remainingTime = 0;

    // Convert screen coordinates to world coordinates for HUD positioning
    Vector2 screenTopLeft = {0, 0};
    Vector2 worldTopLeft = GetScreenToWorld2D(screenTopLeft, cam);

    // Draw Lives - positioned relative to camera view
    int lives = GameInfo::getInstance()->getLives();
    for (int i = 0; i < lives; i++)
    {
        Rectangle srcRec = {0, 0, (float)HUDLives.width, (float)HUDLives.height};
        Vector2 lifePos = {worldTopLeft.x + 10 + i * 15, worldTopLeft.y + 5};
        Rectangle destRec = {lifePos.x, lifePos.y, 12, 12};
        DrawTexturePro(HUDLives, srcRec, destRec, {0, 0}, 0.0f, WHITE);
    }

    // Draw Time icon - positioned relative to camera view
    Rectangle srcRec = {0, 0, (float)HUDTime.width, (float)HUDTime.height};
    Vector2 timeIconPos = {worldTopLeft.x + 100, worldTopLeft.y + 3};
    Rectangle destRec = {timeIconPos.x, timeIconPos.y, 15, 15};
    DrawTexturePro(HUDTime, srcRec, destRec, {0, 0}, 0.0f, WHITE);

    string timeText = to_string(remainingTime);
    Vector2 timeTextPos = {worldTopLeft.x + 120, worldTopLeft.y + 7};

    // Time warning - change color when time is low
    if (remainingTime <= 30)
    {
        DrawTextEx(UI::font, timeText.c_str(), timeTextPos, 12, 2, RED);
    }
    else
    {
        DrawTextEx(UI::font, timeText.c_str(), timeTextPos, 12, 2, WHITE);
    }
    if (remainingTime == 0)
    {
        if (mario) mario->changeState(new DeadState(mario));
        if (luigi) luigi->changeState(new DeadState(luigi));
    }

    // Draw coin
    Rectangle srcRecCoin = {0, 0, (float)HUDCoin.width, (float)HUDCoin.height};
    Vector2 coinIconPos = {worldTopLeft.x + 150, worldTopLeft.y + 3};
    Rectangle destRecCoin = {coinIconPos.x, coinIconPos.y, 18, 18};
    DrawTexturePro(HUDCoin, srcRecCoin, destRecCoin, {0, 0}, 0.0f, WHITE);

    string coinText = to_string(GameInfo::getInstance()->getCoins());
    Vector2 coinTextPos = {worldTopLeft.x + 170, worldTopLeft.y + 7};
    DrawTextEx(UI::font, coinText.c_str(), coinTextPos, 12, 2, WHITE);

    // Draw Score
    Rectangle srcRecScore = {0, 0, (float)HUDScore.width, (float)HUDScore.height};
    Vector2 scoreIconPos = {worldTopLeft.x + 200, worldTopLeft.y + 3};
    Rectangle destRecScore = {scoreIconPos.x, scoreIconPos.y, 18, 18};
    DrawTexturePro(HUDScore, srcRecScore, destRecScore, {0, 0}, 0.0f, WHITE);

    string scoreText = to_string(GameInfo::getInstance()->getScore());
    Vector2 scoreTextPos = {worldTopLeft.x + 220, worldTopLeft.y + 7};
    DrawTextEx(UI::font, scoreText.c_str(), scoreTextPos, 14, 2, YELLOW);
}
