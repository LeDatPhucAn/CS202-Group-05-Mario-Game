#include "Game.hpp"
#include "UI.hpp"

Game::Game(): Mario(
        Character::Builder()
        .setFrames(IDLE, 0, 0)
        .setFrames(WALK, 1, 3)
        .setFrames(JUMP, 5, 5)
        .setFrames(FALL, 5, 5)
        .setFrames(SKID, 4, 4)
        .setFrames(RUN, 1, 3)
		.setFrames(CROUCH, 0, 0)
        .setFrames(GROW, 44, 50)
        .setFrames(UNGROW, 50, 44)
        .setJsonAndTexture("Mario2D")
        .setPos({ 100,0 })
        .build()
    ),
    Goomba(
        Enemy::Builder()
        .setJsonAndTexture("Enemies2D")
        .setFrames(IDLE, 0, 0)
        .setFrames(WALK, 0, 1)
        .setFrames(JUMP, 0, 0)
        .setFrames(FALL, 0, 1)
        .setPos({ 150,0 })
        .build()
    )
{
    mapPaths = {
		{"Map1.1", "assets/Map/Map1.1.json"},
		// Add the rest...
	};
    init();

    MarioPointer = &Mario;
}
void Game::init() {
    //Load assets
    //Load Mari
    current_Map = "Map1.1";
    curMap.choose(mapPaths[current_Map]);

    Mario.changeState(new IdleState(&Mario));
    Goomba.changeState(new IdleState(&Goomba));
}
void Game::updateScene() {
    
    Mario.update();
    Goomba.update();
    curMap.update();
    //Check Collision

    //Mario with Map
    for(auto &x : curMap.tileBlocks) {
        if(MarioPointer->checkCollision(x)) {
            cout << "Collision Mario-Blocks" << endl;
            MarioPointer->updateCollision(x);
            x->updateCollision(MarioPointer);
        }
    }
    

    // //Blocks with Blocks
    // for(int i = 0; i < curMap.tileBlocks.size(); i++) {
    //     for(int j = 0; j < curMap.tileBlocks.size(); j++) {
    //         if(i != j) {
    //             Block* a = curMap.tileBlocks[i];
    //             Block* b = curMap.tileBlocks[j];
    //             if(a->checkCollision(b)) {
    //                 a->updateCollision(b);
    //                 b->updateCollision(a);
    //             }
    //         }
    //     }
    // }

}
void Game::displaySceneInCamera() {
    curMap.display();
    Mario.display();
    Goomba.display();
}
void Game::displayScene() {
}
