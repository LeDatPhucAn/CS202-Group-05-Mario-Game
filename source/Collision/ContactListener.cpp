// MyContactListener.cpp
#include "ContactListener.hpp"
#include <iostream>
#include "Character.hpp"
void ContactListener::BeginContact(b2Contact *contact)
{
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();
    CollisionType typeA = static_cast<CollisionType>(fixtureA->GetUserData().pointer);
    CollisionType typeB = static_cast<CollisionType>(fixtureB->GetUserData().pointer);

    GameObject *objA = reinterpret_cast<GameObject *>(fixtureA->GetBody()->GetUserData().pointer);
    GameObject *objB = reinterpret_cast<GameObject *>(fixtureB->GetBody()->GetUserData().pointer);

    // switch (typeA)
    // {
    // case TOP:
    //     objA->updateCollision(objB, TOP);
    //     break;
    // case BOTTOM:
    //     objA->updateCollision(objB, BOTTOM);
    //     break;
    // case LEFTSIDE:
    //     objA->updateCollision(objB, LEFTSIDE);
    //     break;
    // case RIGHTSIDE:
    //     objA->updateCollision(objB, RIGHTSIDE);
    //     break;
    // }
    // switch (typeB)
    // {
    // case TOP:
    //     objB->updateCollision(objA, TOP);
    //     break;
    // case BOTTOM:
    //     objB->updateCollision(objA, BOTTOM);
    //     break;
    // case LEFTSIDE:
    //     objB->updateCollision(objA, LEFTSIDE);
    //     break;
    // case RIGHTSIDE:
    //     objB->updateCollision(objA, RIGHTSIDE);
    //     break;
    // }
    checkGrounded(contact, true);
}

void ContactListener::EndContact(b2Contact *contact)
{
    // Optional: you can use this if needed
    checkGrounded(contact, false);
}
void ContactListener::checkGrounded(b2Contact *contact, bool began)
{
    b2Fixture *a = contact->GetFixtureA();
    b2Fixture *b = contact->GetFixtureB();

    // Check both fixtures for a BOTTOM sensor
    for (int i = 0; i < 2; i++)
    {
        b2Fixture *sensor = (i == 0) ? a : b;
        b2Fixture *other = (i == 0) ? b : a;

        if (sensor->IsSensor() && sensor->GetUserData().pointer == static_cast<uintptr_t>(CollisionType::BOTTOM))
        {
            b2Body *body = sensor->GetBody();
            Character *character = reinterpret_cast<Character *>(body->GetUserData().pointer);
            if (character)
            {
                if (began)
                    character->groundContacts++;
                else
                    character->groundContacts--;

                character->isGrounded = character->groundContacts > 0;
            }
        }
    }
}