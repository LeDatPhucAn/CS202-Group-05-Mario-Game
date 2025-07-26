#pragma once
#include "Character.hpp"
#include "MarioState.hpp"
#include "UI.hpp"
enum MarioForm
{
    SMALL = 0,
    BIG = 1,
    FIRE,
    FORM_COUNT
};
class Mario : public Character
{
private:
    // Mario-specific friend classes
    friend class MarioState;
    friend class IdleState;
    friend class WalkState;
    friend class RunState;
    friend class JumpState;
    friend class FallState;
    friend class SkidState;
    friend class CrouchState;
    friend class GrowState;
    friend class UnGrowState;
    friend class DeadState;
    friend class Goomba;
    friend class Koopa;

    MarioForm form = SMALL;
    void changeForm(MarioForm form);

public:
    Mario();
    // Override collision to handle enemies
    void updateCollision(GameObject *other, int type) override;
    void createBody(b2World *world) override;

public:
};