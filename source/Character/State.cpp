#include "State.hpp"
#include "Character.hpp"
#include "Game.hpp"
#include "Program.hpp"

string stateTypeToString(stateType state)
{
    switch (state)
    {
    case stateType::IDLE:
        return "IDLE";
    case stateType::WALK:
        return "WALK";
    case stateType::RUN:
        return "RUN";
    case stateType::JUMP:
        return "JUMP";
    case stateType::FALL:
        return "FALL";
    case stateType::SKID:
        return "SKID";
    case stateType::CROUCH:
        return "CROUCH";
    case stateType::GROW:
        return "GROW";
    case stateType::UNGROW:
        return "UNGROW";
    default:
        return "UNKNOWN";
    }
}
// ---------- Base State ----------
State::State() : delay(0), character(nullptr), delayCounter(0), frameIndex(0), type(IDLE), numFrames(0) {}

State::State(stateType Type, Character *_character, int _delay)
    : delay(_delay), character(_character), delayCounter(_delay), type(Type), frameIndex(0)
{
    StartEndFrame se = character->sprite.StartEndFrames[Type];
    numFrames = abs(se.end - se.start) + 1;
    frameRec = character->sprite.frameRecs[se.start];
    frameRec.width = character->direction * abs(frameRec.width);
    character->setSize({fabs(frameRec.width), frameRec.height});
}
void State::animate()
{
    delayCounter++;
    if (delayCounter > delay)
    {
        delayCounter = 0;
        frameIndex = (frameIndex + 1) % numFrames;
        StartEndFrame se = character->sprite.StartEndFrames[type];
        frameRec = (se.start <= se.end) ? character->sprite.frameRecs[se.start + frameIndex] : character->sprite.frameRecs[se.start - frameIndex];
    }
}

void State::updateState()
{
    // Update the character's position based on Box2D body
    Vec2Adapter adapter(character->body->GetPosition());
    character->setPositionAdapter(adapter);

    // Updates the character's hitbox based on the current frame
    if (character->changeBody)
    {
        Game::world->DestroyBody(character->getBody());
        character->createBody(Game::world);
        character->changeBody = false;
    }

    Vec2Adapter mouse(GetMousePosition());

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && dynamic_cast<Mario *>(character))
    {
        // Teleport the Box2D body
        character->body->SetTransform(mouse.toMeters(), 0); // 0 = angle

        std::cout << "Teleported to: " << mouse.toPixels().x << ", " << mouse.toPixels().y << "\n";
    }
    else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && dynamic_cast<Mario *>(character))
    {
        Koopa *newKoopa = new Koopa();
        newKoopa->setPosition(mouse.toPixels());
        newKoopa->changeState(new EnemyWalkState(newKoopa));
        Game::addEnemy(newKoopa);
    }
    // Set frame
    animate();

    // change character direction accordingly
    frameRec.width = character->direction * abs(frameRec.width);

    // apply different gravity when jumping and falling
    if (!character->isGrounded)
    {
        float gravityAccel = (character->body->GetLinearVelocity().y < 0 && (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)))
                                 ? jumpGravity
                                 : addedFallGravity;
        character->body->ApplyForceToCenter({0, character->body->GetMass() * gravityAccel}, true);
    }

    // Change State when pressing
    handleInput();
}
void State::DrawCharacterDebug(Character *character)
{
    b2Body *body = character->getBody();
    for (b2Fixture *f = body->GetFixtureList(); f; f = f->GetNext())
    {
        b2Shape::Type type = f->GetType();
        // Choose color based on sensor type
        Color color = RED;
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
            color = RED;
            break; // main body
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
                DrawLineEx(points[i], points[(i + 1) % count], 0.5f, color);
            }
        }

        else if (type == b2Shape::e_circle)
        {
            // Draw foot sensor circle
            b2CircleShape *circle = (b2CircleShape *)f->GetShape();
            b2Vec2 worldCenter = body->GetWorldPoint(circle->m_p);
            float radius = circle->m_radius;

            int screenX = static_cast<int>(worldCenter.x * PPM);
            int screenY = static_cast<int>(worldCenter.y * PPM);
            int screenRadius = static_cast<int>(radius * PPM);

            DrawCircleLines(screenX, screenY, screenRadius, color);
        }
    }
}

void State::displayState()
{
    DrawCharacterDebug(character);

    DrawTextureRec(character->sprite.texture, frameRec, character->getCenter(), WHITE);
}
