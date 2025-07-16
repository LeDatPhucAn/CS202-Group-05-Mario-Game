// MyContactListener.hpp
#pragma once
#include <box2d/box2d.h>
#include "GameObject.hpp"

class ContactListener : public b2ContactListener
{
private:
    void checkGrounded(b2Contact *contact, bool began);

public:
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
};
