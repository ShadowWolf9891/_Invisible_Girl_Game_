#pragma once
#ifndef Raycast_h
#define Raycast_h

#include "Object.h"
#include "S_Collidable.h"

struct RaycastResult
{
    Object* collision;
    CollisionLayer layer;
};

class Raycast
{
public:
    Raycast(Quadtree& collisions);

    RaycastResult Cast(const sf::Vector2f& from, const sf::Vector2f& to, int exclusionID = -1);
    RaycastResult Cast(const sf::Vector2f& from, const sf::Vector2f& to, CollisionLayer layer);

private:
    sf::FloatRect BuildRect(const sf::Vector2f& lineOne, const sf::Vector2f& lineTwo);
    std::vector<sf::Vector2f> BuildLinePoints(const sf::Vector2f& from, const sf::Vector2f& to);

    Quadtree& collisions;
};

#endif /* Raycast_h */