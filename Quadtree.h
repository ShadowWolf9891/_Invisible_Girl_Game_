#pragma once
#ifndef QuadTree_h
#define QuadTree_h

#include <memory>
#include <vector>

#include "C_BoxCollider.h"
#include "Object.h"
#include "Debug.h"

class Quadtree
{
public:
	Quadtree();
	Quadtree(int maxObjects, int maxLevels, int level, sf::FloatRect bounds, Quadtree* parent);
	
	void DrawDebug();

	
	void Insert(std::shared_ptr<C_BoxCollider> object);// Inserts object into our quadtree.
	void Remove(std::shared_ptr<C_BoxCollider> object);// Removes object from our quadtree when we no longer need it to collide.
	void Clear();// Removes all objects from tree.
	void UpdatePosition(std::shared_ptr<C_BoxCollider> object); //Updates the position of an object within the tree

	std::vector<std::shared_ptr<C_BoxCollider>> Search(const sf::FloatRect& area);// Returns vector of colliders that intersect with the search area.
	
	const sf::FloatRect& GetBounds() const;// Returns the bounds of this node.
	//Drawing for debugging
	
private:

	void Search(const sf::FloatRect& area, std::vector<std::shared_ptr<C_BoxCollider>>& overlappingObjects);

	// Returns the index for the node that will contain the object. -1 is returned if it is this node.
	int GetChildIndexForObject(const sf::FloatRect& objectBounds);
	// Creates the child nodes.
	void Split();

	// TODO: Enum might be a better way to store the data
	static const int thisTree = -1;
	static const int childNE = 0;
	static const int childNW = 1;
	static const int childSW = 2;
	static const int childSE = 3;

	int maxObjects;
	int maxLevels;
	// nulptr is this is the base node.
	Quadtree* parent;
	std::shared_ptr<Quadtree> children[4];

	// Stores objects in this node.
	std::vector<std::shared_ptr<C_BoxCollider>> objects;
	// How deep the current node is from the base node. 
	// The first node starts at 0 and then its child node is at level 1 and so on.
	int level;
	// The bounds of this node.
	sf::FloatRect bounds;
};

#endif /* QuadTree_h */