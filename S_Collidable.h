#pragma once
#ifndef S_Collidable_h
#define S_Collidable_h

#include <vector>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "Object.h"
#include "Quadtree.h"
#include "Bitmask.h"
#include "ComponentPairHash.h"


class S_Collidable
{
public:
	S_Collidable(Quadtree& collisionTree);

	void Add(std::vector<std::shared_ptr<Object>>& objects);
	void ProcessRemovals();
	void UpdatePositions(std::vector<std::shared_ptr<Object>>& objects);
	void Resolve();

	void Update();

private:
	void ProcessCollidingObjects();

	
	std::unordered_map<CollisionLayer, Bitmask> collisionLayers; // This is used to store collision layer data i.e. which layers can collide.
	std::unordered_map<CollisionLayer, std::vector<std::shared_ptr<C_BoxCollider>>> collidables;// The collision system stores all collidables along with their layer.
	std::unordered_set<std::pair<std::shared_ptr<C_BoxCollider>, std::shared_ptr<C_BoxCollider>>, ComponentPairHash> objectsColliding;

	// The quadtree stores the collidables in a spatial aware structure.
	Quadtree& collisionTree;
};

#endif /* S_Collidable_h */