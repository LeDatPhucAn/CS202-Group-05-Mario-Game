#pragma once
#include "raylib.h"
#include "box2d/box2d.h"
#include "Vec2Adapter.hpp"
#include "Structs.hpp"
class DrawDebug
{
public:
    void DrawWorld(b2World *world)
    {
        for (b2Body *b = world->GetBodyList(); b; b = b->GetNext())
        {
            for (b2Fixture *f = b->GetFixtureList(); f; f = f->GetNext())
            {
                // Get collision type and color
                auto type = static_cast<CollisionType>(f->GetUserData().pointer);
                Color c = ColorForCollisionType(type);

                // Get shape and transform
                b2Shape::Type shapeType = f->GetType();
                b2Transform xf = b->GetTransform();

                if (shapeType == b2Shape::e_circle)
                {
                    auto *circle = static_cast<b2CircleShape *>(f->GetShape());
                    b2Vec2 center = b2Mul(xf, circle->m_p);
                    float r = circle->m_radius;
                    Vector2 p = Vec2Adapter(center).toPixels();
                    DrawCircleLinesV(p, r * PPM, c);
                }
                else if (shapeType == b2Shape::e_polygon)
                {
                    auto *poly = static_cast<b2PolygonShape *>(f->GetShape());
                    int32 count = poly->m_count;

                    // Compute world-space and pixel-space points
                    Vector2 pts[b2_maxPolygonVertices];
                    for (int32 i = 0; i < count; ++i)
                    {
                        b2Vec2 v = b2Mul(xf, poly->m_vertices[i]);
                        pts[i] = Vec2Adapter(v).toPixels();
                    }

                    // Compute polygon center (centroid)
                    Vector2 center = {0.0f, 0.0f};
                    for (int32 i = 0; i < count; ++i)
                    {
                        center.x += pts[i].x;
                        center.y += pts[i].y;
                    }
                    center.x /= count;
                    center.y /= count;

                    // Draw filled polygon using triangle fan
                    for (int32 i = 0; i < count; ++i)
                    {
                        int32 j = (i + 1) % count;
                        DrawTriangle(
                            pts[i],
                            pts[j],
                            center,
                            Fade(c, 0.3f));
                    }

                    // Draw polygon outline
                    for (int32 i = 0; i < count; ++i)
                    {
                        int32 j = (i + 1) % count;
                        DrawLineV(pts[i], pts[j], c);
                    }
                }
                // TODO: handle other shape types (edge, chain) if needed
            }
        }
    }

private:
    Color B2ColorToRaylib(const b2Color &color)
    {
        return {(unsigned char)(color.r * 255), (unsigned char)(color.g * 255),
                (unsigned char)(color.b * 255), 255};
    }
    static Color ColorForCollisionType(CollisionType type)
    {
        switch (type)
        {
        case CollisionType::NONE:
            return RED;
        case CollisionType::BOTTOM:
            return GREEN;
        case CollisionType::TOP:
            return BLUE;
        case CollisionType::LEFTSIDE:
            return PURPLE;
        case CollisionType::RIGHTSIDE:
            return YELLOW;
        // … your other cases …
        default:
            return MAGENTA;
        }
    }
};
