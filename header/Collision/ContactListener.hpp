// MyContactListener.hpp
#pragma once
#include <box2d/box2d.h>
#include "GameObject.hpp"

class ContactListener : public b2ContactListener
{
private:
    template <typename T, typename U>
    bool MatchPair(GameObject *a, GameObject *b)
    {
        return (dynamic_cast<T *>(a) && dynamic_cast<U *>(b)) ||
               (dynamic_cast<U *>(a) && dynamic_cast<T *>(b));
    }

private:
    void checkGrounded(b2Contact *contact, bool began);
    bool ShouldNotCollide(GameObject *objA, GameObject *objB);

public:
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
    void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) override;
};
