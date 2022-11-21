#pragma once
#ifndef C_SteeringBehaviour_h
#define C_SteeringBehaviour_h

#include "Component.h"
#include "C_Tag.h"
#include "SharedContext.h"
#include "Quadtree.h"
class C_SteeringBehaviour : public Component
{
public:
    C_SteeringBehaviour(Object* owner) : Component(owner), weight(1) {};

    virtual const sf::Vector2f GetForce() = 0;

    unsigned int GetWeight() const;
    void SetWeight(unsigned int weight);

protected:
    Object* GetEntityInSight(float sightRadius, Tag tag) const;
    std::vector<Object*> GetEntitiesInSight(float sightRadius, Tag tag) const;

    //TODO: move to separate class.
    inline sf::Vector2f Normalise(const sf::Vector2f& v)
    {
        float l = Magnitude(v);

        if (l != 0)
        {
            return v / l;
        }

        return v;
    }

    //TODO: move to separate class.
    inline float Magnitude(const sf::Vector2f& v)
    {
        return sqrt((v.x * v.x) + (v.y * v.y));
    }

private:
    unsigned int weight;
};

#endif /* C_SteeringBehaviour_h */