#pragma once
#include "raylib-tileson.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
using namespace std;

class MyMap {
public:
    MyMap() = default;
    inline void Init();
    inline void Display(const string& name_map);
    inline void Update();

    inline ~MyMap();

private:
    unordered_map<string, Map> gameMap; 
};

void MyMap::Init() {
	std::unordered_map<std::string, std::string> mapPaths = {
		{"map1", "assets/Map/Map1.1.json"},
		// Add the rest...
	};
	
	for (const auto& KeyAndPath : mapPaths) {
		gameMap[KeyAndPath.first] = LoadTiled(KeyAndPath.second.c_str());
	}
    cout << "ok";
}

void MyMap::Display(const string& name_map) {
    DrawTiled(gameMap[name_map], 0, 0, WHITE);
}

void MyMap::Update() {

}

MyMap::~MyMap() {
    for(const auto &x : gameMap){
		UnloadMap(x.second);
	}
}