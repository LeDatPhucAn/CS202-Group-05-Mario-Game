#include "MyMap.hpp"

Vector2 MyMap::GetStartingPoint() {
    tson::Layer *layer = tsonMap->getLayer("objectgroup");
    tson::Object *point = layer->firstObj("StartingPoint");
    float x = point->get<float>("x");
    float y = layer->get<float>("y");
    return {x,y};
}

Vector2 MyMap::GetFinishingPoint() {
    tson::Layer *layer = tsonMap->getLayer("objectgroup");
    tson::Object *point = layer->firstObj("Finishing Point");
    float x = point->get<float>("x");
    float y = layer->get<float>("y");
    return {x,y};
}
