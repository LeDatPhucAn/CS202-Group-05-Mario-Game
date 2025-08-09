#pragma once
#include "Mario.hpp"

class PlayerMario : public Mario
{
public:
    PlayerMario() : Mario()
    {
        setTexture("Mario2D");
        changeState(new IdleState(this));

        // Press A to move left
        bindKey(Action::BUTTON_LEFT, KEY_A);

        // Press D to move right
        bindKey(Action::BUTTON_RIGHT, KEY_D);

        // Press W to look/jump up (or climb ladders, if implemented)
        bindKey(Action::BUTTON_UP, KEY_W);

        // Press S to crouch or move down
        bindKey(Action::BUTTON_DOWN, KEY_S);

        // Hold Left Shift to run
        bindKey(Action::BUTTON_RUN, KEY_LEFT_SHIFT);

        // Press F to throw a fireball
        bindKey(Action::BUTTON_THROW_FIREBALL, KEY_F);
    }
};

class PlayerLuigi : public Mario
{
public:
    PlayerLuigi() : Mario()
    {
        setTexture("Luigi2D");
        changeState(new IdleState(this));

        // Press Left Arrow to move lefts
        bindKey(Action::BUTTON_LEFT, KEY_LEFT);

        // Press Right Arrow to move right
        bindKey(Action::BUTTON_RIGHT, KEY_RIGHT);

        // Press Up Arrow to look/jump up (or climb ladders, if implemented)
        bindKey(Action::BUTTON_UP, KEY_UP);

        // Press Down Arrow to crouch or move down
        bindKey(Action::BUTTON_DOWN, KEY_DOWN);

        // Hold Right Shift to run
        bindKey(Action::BUTTON_RUN, KEY_RIGHT_SHIFT);

        // Press Slash (/) to throw a fireball
        bindKey(Action::BUTTON_THROW_FIREBALL, KEY_SLASH);
    }
};
