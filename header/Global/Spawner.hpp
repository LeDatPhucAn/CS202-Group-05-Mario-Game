#pragma once
#include <iostream>
#include <raylib.h>
#include <unordered_map>
#include "Goomba.hpp"
#include "Koopa.hpp"
#include "PiranhaPlant.hpp"
#include "Lakitu.hpp"
#include "Spiny.hpp"
#include "BulletBill.hpp"
#include "HammerBro.hpp"
#include "Game.hpp"
using namespace std;

//Spawner Factory
class Spawner {
public:
    
    Spawner(Game * _game) : game(_game) {}
    ~Spawner() { game = nullptr;} 

    void spawnEnemy() {
        for (auto itInner = InfoSpawn["Enemy"].begin(); 
             itInner != InfoSpawn["Enemy"].end(); 
             ++itInner)
        {
            const string& typeName = itInner->first;
            const vector<Vector2>& positions = itInner->second;

            // Duyệt qua từng vị trí và spawn
            for (const auto& pos : positions)
            {
                if(typeName == "Goomba") {
                    Goomba* tmp = new Goomba();
                    tmp->setPosition(pos);
                    game->addGameObject(tmp);
                }
                if(typeName == "Koopa") {
                    Koopa* tmp = new Koopa();
                    tmp->setPosition(pos);
                    game->addGameObject(tmp);
                }
                if(typeName == "PiranhaPlant") {
                    PiranhaPlant* tmp = new PiranhaPlant();
                    tmp->setPosition(pos);
                    game->addGameObject(tmp);
                }
                if(typeName == "Lakitu") {
                    Lakitu* tmp = new Lakitu();
                    tmp->setPosition(pos);
                    game->addGameObject(tmp);
                }
                if(typeName == "BulletBill") {
                    BulletBill* tmp = new BulletBill();
                    tmp->setPosition(pos);
                    game->addGameObject(tmp);
                }
                
                // if(typeName == "HammerBro") {
                //     HammerBro* tmp = new HammerBro();
                //     tmp->setPosition(pos);
                //     game->addGameObject(tmp);
                // }
            }
        }
    }
    //[Enemy/Item][Name_of_Obj][...vectorPos..]
    unordered_map<string, unordered_map<string, vector<Vector2>>>  InfoSpawn;

private:
    Game* game;
};