#pragma once
#ifndef ObjectCollection_h
#define ObjectCollection_h

#include <memory>
#include <vector>

#include "Object.h"
#include "S_Drawable.h"
#include "S_Collidable.h"

class ObjectCollection
{
public:
	ObjectCollection(S_Drawable& drawableSystem, S_Collidable& collidableSystem);
	void Add(std::shared_ptr<Object> object);
	void Add(std::vector<std::shared_ptr<Object>>& objects);

	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void Draw(Window& window);

	void ProcessNewObjects(); // In its own method so it can run at a convieniant time.
	void ProcessRemovals();
private:
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<Object>> newObjects; //This is used to temporarily store recently added objects to choose when they are added to the game
	
	S_Drawable& drawables;
	S_Collidable& collidables;
};

#endif /* ObjectCollection_h */