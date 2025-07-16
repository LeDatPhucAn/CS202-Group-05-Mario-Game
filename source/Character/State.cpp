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
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = Program::mouseWorldPos;

        // Convert screen position to Box2D world coordinates
        float box2dX = mouse.x / PPM;
        float box2dY = mouse.y / PPM;

        // Teleport the Box2D body
        character->body->SetTransform({box2dX, box2dY}, 0); // 0 = angle

        // Optional: zero out any movement velocity
        character->body->SetLinearVelocity({0, 0});

        // Sync sprite position (optional, will get overridden next frame anyway)
        character->pos = mouse;

        std::cout << "Teleported to: " << box2dX << ", " << box2dY << "\n";
    }
    // Set frame
    animate();

    // change character direction accordingly
    frameRec.width = character->direction * abs(frameRec.width);

    // apply different gravity when jumping and falling
    if (!character->isGrounded)
    {
        float gravityAccel = (character->body->GetLinearVelocity().y < 0 && IsKeyDown(KEY_UP))
                                 ? jumpGravity
                                 : fallGravity;

        character->body->ApplyForceToCenter({0, character->body->GetMass() * gravityAccel}, true);
    }

    // Change State when pressing
    handleInput();
}
void DrawCharacterDebug(Character *character)
{
    b2Body *body = character->getBody();
    b2Transform transform = body->GetTransform();

    for (b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        if (fixture->GetType() != b2Shape::e_polygon)
            continue;

        b2PolygonShape *poly = (b2PolygonShape *)fixture->GetShape();

        Vector2 screenVerts[8]; // up to 8 vertices
        for (int i = 0; i < poly->m_count; i++)
        {
            // Transform from local to world
            b2Vec2 worldVert = b2Mul(transform, poly->m_vertices[i]);
            Vec2Adapter adapter(worldVert);

            // Convert to screen (pixels)
            screenVerts[i] = adapter.toPixels();
        }

        // Choose color based on sensor type
        Color color = RED;
        CollisionType type = (CollisionType)fixture->GetUserData().pointer;
        switch (type)
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

        // Draw polygon outline
        for (int i = 0; i < poly->m_count; i++)
        {
            int j = (i + 1) % poly->m_count;
            DrawLine((int)screenVerts[i].x, (int)screenVerts[i].y,
                     (int)screenVerts[j].x, (int)screenVerts[j].y, color);
        }
    }
}

void State::displayState()
{
    DrawCharacterDebug(character);
    Vec2Adapter adapter(character->body->GetPosition());
    DrawTextureRec(character->sprite.texture, frameRec, adapter.toPixels(), WHITE);
}
