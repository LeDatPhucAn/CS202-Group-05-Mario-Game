#include "State.hpp"
#include "Game.hpp"
#include "Character.hpp"
#include "Program.hpp"
// ---------- Base State ----------
State::State() : delay(0), gameObject(nullptr), delayCounter(0), frameIndex(0), numFrames(0) {}

State::State(int _type, GameObject *_gameObject, int _delay)
    : delay(_delay), gameObject(_gameObject), delayCounter(_delay), type(_type), frameIndex(0)
{
    StartEndFrame se = gameObject->sprite.StartEndFrames[_type];
    numFrames = abs(se.end - se.start) + 1;
    frameRec = gameObject->sprite.frameRecs[se.start];
    frameRec.width = gameObject->direction * abs(frameRec.width);
    gameObject->setSize({fabs(frameRec.width), frameRec.height});
}

void State::animate()
{
    delayCounter++;
    if (delayCounter > delay)
    {
        delayCounter = 0;
        frameIndex = (frameIndex + 1) % numFrames;
        StartEndFrame se = gameObject->sprite.StartEndFrames[type];
        frameRec = (se.start <= se.end) ? gameObject->sprite.frameRecs[se.start + frameIndex] : gameObject->sprite.frameRecs[se.start - frameIndex];
    }
}

void State::updateState()
{
    // Update the gameObject's position based on Box2D body
    Vec2Adapter adapter(gameObject->body->GetPosition());
    gameObject->setPositionAdapter(adapter);

    // Check if the character's body needs to be changed
    if (dynamic_cast<Character *>(gameObject))
    {
        Character *character = static_cast<Character *>(gameObject);
        if (character->changeBody)
        {
            // Destroy the old body and create a new one
            Game::world->DestroyBody(character->getBody());
            character->createBody(Game::world);
            character->changeBody = false;
        }
        // apply different gravity when jumping and falling
        if (!character->isGrounded)
        {
            float gravityAccel = (character->body->GetLinearVelocity().y < 0 && (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)))
                                     ? jumpGravity
                                     : addedFallGravity;
            character->body->ApplyForceToCenter({0, character->body->GetMass() * gravityAccel}, true);
        }
    }

    // Update the gameObject's position
    gameObjectDebug(gameObject);

    // Set frame
    animate();

    // change character direction accordingly
    frameRec.width = gameObject->direction * abs(frameRec.width);

    // Change State when pressing
    handleInput();
}

void State::displayState()
{
    drawDebug(gameObject);
    DrawTextureRec(gameObject->sprite.texture, frameRec, gameObject->getCenter(), WHITE);
}
void State::gameObjectDebug(GameObject *gameObject)
{
    // Vec2Adapter mouse(Program::mouseWorldPos);

    // if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && dynamic_cast<Mario *>(gameObject))
    // {
    //     // Teleport the Box2D body
    //     gameObject->body->SetTransform(mouse.toMeters(), 0); // 0 = angle

    //     std::cout << "Teleported to: " << mouse.toPixels().x << ", " << mouse.toPixels().y << "\n";
    // }
    // else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && dynamic_cast<Mario *>(gameObject))
    // {
    //     Koopa *newKoopa = new Koopa();
    //     newKoopa->setPosition(mouse.toPixels());
    //     newKoopa->changeState(new EnemyWalkState(newKoopa));
    //     Game::addEnemy(newKoopa);
    // }
}
void State::drawDebug(GameObject *gameObject)
{
    // Draw the hitbox of the gameObject
    b2Body *body = gameObject->getBody();
    if (!body)
        return;

    for (b2Fixture *f = body->GetFixtureList(); f; f = f->GetNext())
    {
        b2Shape::Type type = f->GetType();
        Color color;

        // Determine color based on fixture type
        CollisionType Type = static_cast<CollisionType>(f->GetUserData().pointer);
        switch (Type)
        {
        case CollisionType::TOP:
            color = BLUE;
            break;
        case CollisionType::BOTTOM:
            color = GREEN;
            break;
        case CollisionType::LEFTSIDE:
            color = ORANGE;
            break;
        case CollisionType::RIGHTSIDE:
            color = PURPLE;
            break;
        default:
            color = RED; // Main body
            break;
        }
        if (type == b2Shape::e_polygon)
        {
            // Draw polygon fixtures (like main body)
            b2PolygonShape *poly = (b2PolygonShape *)f->GetShape();
            int count = poly->m_count;
            Vector2 points[8];

            for (int i = 0; i < count; i++)
            {
                Vec2Adapter world(body->GetWorldPoint(poly->m_vertices[i]));
                points[i] = world.toPixels();
            }

            for (int i = 0; i < count; i++)
            {
                DrawLineV(points[i], points[(i + 1) % count], color);
            }
        }
        else if (type == b2Shape::e_circle)
        {
            // Draw circle fixtures
            b2CircleShape *circle = (b2CircleShape *)f->GetShape();
            Vec2Adapter center(body->GetWorldPoint(circle->m_p));
            float radius = circle->m_radius * PPM;
            DrawCircleLinesV(center.toPixels(), radius, color);
        }
    }
}
