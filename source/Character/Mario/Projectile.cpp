#include "Projectile.hpp"

Projectile::Projectile() : GameObject()
{
    this->sprite.frameRecs = UI::JsonToRectangleVector(UI::jsonMap["Projectiles2D"]);
    this->sprite.texture = UI::textureMap["Projectiles2D"];
}
void Projectile::update()
{
    // set the position based on Box2D body
    Vec2Adapter adapter(body->GetPosition());
    setPositionAdapter(adapter);

    // Update logic for the projectile
    if (body)
    {
        pos = Vec2Adapter(body->GetPosition()).toPixels();
        size = {16, 16}; // Assuming a fixed size for the projectile
    }
}
void Projectile::display()
{
    DrawTextureRec(sprite.texture, sprite.frameRecs[0], pos.toPixels(), WHITE); // Display the first frame
}
void Projectile::updateCollision(GameObject *other, int type)
{
    // Handle collision logic here
    if (type == 1) // Example type for collision handling
    {
        // Logic for when the projectile collides with something
        // For example, destroy the projectile or apply effects
    }
}
void Projectile::createBody(b2World *world)
{
    float posX = pos.toMeters().x;
    float posY = pos.toMeters().y;
    float halfWidth = size.getHalfWidth();
    float halfHeight = size.getHalfHeight();

    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_staticBody;

    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    bodyDef.position.Set(posX, posY);
    body = world->CreateBody(&bodyDef);

    // Main box collider
    b2PolygonShape boxShape;
    boxShape.SetAsBox(halfWidth, halfHeight);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.friction = 0.8f;
    fixtureDef.density = 1.0f;
    fixtureDef.restitution = 0.5f; // Add some bounce effect
    body->CreateFixture(&fixtureDef);
}