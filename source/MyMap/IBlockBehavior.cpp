#include "IBlockBehavior.hpp"
#include "raylib.h"
#include "Block.hpp"
#include "Particle.hpp"
#include "Game.hpp"
#include "Mario.hpp"
#include "GameObject.hpp"


//type của Mario đối với Block, Type = TOP là MArio nhảy lên đụng block
void QuestionBehavior::reactToCollision(GameObject* p, int type) {
    Mario* mario = dynamic_cast<Mario*>(p);
    if (!mario) return;

    if(type == BOTTOM) {
        cout << "Question Block hit!" << endl;
        block->isJumping = true;
    }
}



void QuestionBehavior::updateFrame(float dt) {
    if(block->isJumping) {
        block->handleInput(dt);
        block->applyPhysics(dt);
        //spawn mushroom
    }
}

void BrickBehavior::reactToCollision(GameObject* p, int type) {
    // chỉ phản ứng khi va chạm với Mario từ dưới lên
    Mario* mario = dynamic_cast<Mario*>(p);
    if (!mario) return;
    // FEET = đụng từ dưới
    if (type == BOTTOM) {
        block->needDeletion = true;
        Particle::spawnParticles(*block, Game::particles);
    }
}

void BrickBehavior::updateFrame(float dt) {

}

void BrickBehavior::onDraw(float dt) {
}

void GroundBehavior::reactToCollision(GameObject* p, int type) {

}
