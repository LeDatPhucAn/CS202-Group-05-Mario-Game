// MyContactListener.cpp
#include "ContactListener.hpp"

void ContactListener::BeginContact(b2Contact *contact)
{
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();
    CollisionType typeA = static_cast<CollisionType>(fixtureA->GetUserData().pointer);
    CollisionType typeB = static_cast<CollisionType>(fixtureB->GetUserData().pointer);

    GameObject *objA = reinterpret_cast<GameObject *>(fixtureA->GetBody()->GetUserData().pointer);
    GameObject *objB = reinterpret_cast<GameObject *>(fixtureB->GetBody()->GetUserData().pointer);

    switch (typeA)
    {
    case HEAD:
        objA->updateCollision(objB, HEAD);
        break;
    case FEET:
        objA->updateCollision(objB, FEET);
        break;
    case LEFTSIDE:
        objA->updateCollision(objB, LEFTSIDE);
        break;
    case RIGHTSIDE:
        objA->updateCollision(objB, RIGHTSIDE);
        break;
    }
}

void ContactListener::EndContact(b2Contact *contact)
{
    // Optional: you can use this if needed
}
