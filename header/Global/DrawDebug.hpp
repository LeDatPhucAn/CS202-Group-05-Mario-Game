#pragma once
#include "raylib.h"
#include "box2d/box2d.h"
#include "Vec2Adapter.hpp"
#include "Structs.hpp"
class DrawDebug
{
public:
    // void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override
    // {
    //     Color rayColor = B2ColorToRaylib(color);
    //     for (int32 i = 0; i < vertexCount; ++i)
    //     {
    //         Vector2 v1 = Vec2Adapter(vertices[i]).toPixels();
    //         Vector2 v2 = Vec2Adapter(vertices[(i + 1) % vertexCount]).toPixels();
    //         DrawLineV(v1, v2, rayColor);
    //     }
    // }

    // void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override
    // {
    //     Vector2 center = {0};
    //     for (int32 i = 0; i < vertexCount; ++i)
    //     {
    //         center.x += Vec2Adapter(vertices[i]).toPixels().x;
    //         center.y += Vec2Adapter(vertices[i]).toPixels().y;
    //     }
    //     center.x /= vertexCount;
    //     center.y /= vertexCount;

    //     Color fillColor = B2ColorToRaylib(color);
    //     fillColor.a = 80; // Semi-transparent

    //     for (int32 i = 0; i < vertexCount; ++i)
    //     {
    //         Vector2 v1 = Vec2Adapter(vertices[i]).toPixels();
    //         Vector2 v2 = Vec2Adapter(vertices[(i + 1) % vertexCount]).toPixels();
    //         DrawTriangle(center, v1, v2, fillColor);
    //     }

    //     DrawPolygon(vertices, vertexCount, color); // Draw outline
    // }

    // void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override
    // {
    //     Vector2 pos = Vec2Adapter(center).toPixels();
    //     DrawCircleLinesV(pos, radius * PPM, B2ColorToRaylib(color));
    // }

    // void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override
    // {
    //     Vector2 pos = Vec2Adapter(center).toPixels();
    //     float r = radius * PPM;

    //     Color fillColor = B2ColorToRaylib(color);
    //     fillColor.a = 80;
    //     DrawCircleV(pos, r, fillColor);

    //     DrawCircleLinesV(pos, r, B2ColorToRaylib(color));

    //     Vector2 axisEnd = {pos.x + axis.x * r, pos.y + axis.y * r};
    //     DrawLineV(pos, axisEnd, B2ColorToRaylib(color));
    // }

    // void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override
    // {
    //     DrawLineV(Vec2Adapter(p1).toPixels(), Vec2Adapter(p2).toPixels(), B2ColorToRaylib(color));
    // }

    // void DrawTransform(const b2Transform &xf) override
    // {
    //     float lineLength = 0.5f; // meters
    //     b2Vec2 p1 = xf.p;
    //     b2Vec2 p2;

    //     // X-axis (red)
    //     p2 = p1 + lineLength * xf.q.GetXAxis();
    //     DrawLineV(Vec2Adapter(p1).toPixels(), Vec2Adapter(p2).toPixels(), RED);

    //     // Y-axis (green)
    //     p2 = p1 + lineLength * xf.q.GetYAxis();
    //     DrawLineV(Vec2Adapter(p1).toPixels(), Vec2Adapter(p2).toPixels(), GREEN);
    // }
    // void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) override
    // {
    //     Vector2 pos = Vec2Adapter(p).toPixels();
    //     DrawCircleV(pos, size, B2ColorToRaylib(color));
    // }
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
