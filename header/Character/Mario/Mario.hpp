#pragma once
#include "Character.hpp"
#include "MarioState.hpp"

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
    Mario() = default;
    Mario(const Sprite &_sprite, State *_initialState, Vector2 _pos)
        : Character(_sprite, _initialState, _pos) {}

    // Override collision to handle enemies
    void updateCollision(GameObject *other, int type) override;

public:
    // A specific builder for Mario
    struct Builder
    {
        Sprite sprite;
        State *state = nullptr;
        Vector2 pos = {0, 0};

        Builder &setFrames(stateType type, int start, int end)
        {
            sprite.StartEndFrames[type] = {start, end};
            return *this;
        }
        Builder &setJsonAndTexture(string name)
        {
            sprite.texture = UI::textureMap[name];
            sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap[name]);
            return *this;
        }
        Builder &setPos(Vector2 _pos)
        {
            pos = _pos;
            return *this;
        }
        Builder &setState(State *initialState)
        {
            state = initialState;
            return *this;
        }
        Mario build()
        {
            return Mario(sprite, state, pos);
        }
    };
};