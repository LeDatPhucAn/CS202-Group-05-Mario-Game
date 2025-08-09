#pragma once
#include "Mario.hpp"

class PlayerMario : public Mario
{
public:
    PlayerMario() : Mario()
    {
        setTexture("Mario2D");
        changeState(new IdleState(this));

        bindKey(Action::BUTTON_LEFT, KEY_A);
        bindKey(Action::BUTTON_RIGHT, KEY_D);
        bindKey(Action::BUTTON_UP, KEY_W);
        bindKey(Action::BUTTON_DOWN, KEY_S);
        bindKey(Action::BUTTON_RUN, KEY_LEFT_SHIFT);
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

        bindKey(Action::BUTTON_LEFT, KEY_LEFT);
        bindKey(Action::BUTTON_RIGHT, KEY_RIGHT);
        bindKey(Action::BUTTON_UP, KEY_UP);
        bindKey(Action::BUTTON_DOWN, KEY_DOWN);
        bindKey(Action::BUTTON_RUN, KEY_RIGHT_SHIFT);
        bindKey(Action::BUTTON_THROW_FIREBALL, KEY_SLASH);
    }
};