#include "ObjectCollection.h"

ObjectCollection::ObjectCollection(S_Drawable& drawableSystem, S_Collidable& collidableSystem) : drawables(drawableSystem), collidables(collidableSystem) 
{

}

void ObjectCollection::Add(std::shared_ptr<Object> object)
{
	newObjects.push_back(object);
}

void ObjectCollection::Add(std::vector<std::shared_ptr<Object>>& otherObjects)
{
	newObjects.insert(newObjects.end(), otherObjects.begin(), otherObjects.end());
}

void ObjectCollection::Update(float deltaTime)
{
	for (auto& o : objects)
	{
		o->Update(deltaTime);
	}
	collidables.Update();
}

void ObjectCollection::LateUpdate(float deltaTime)
{
	for (auto& o : objects)
	{
		o->LateUpdate(deltaTime);
	}
}

void ObjectCollection::Draw(Window& window)
{
	drawables.Draw(window);
}

void ObjectCollection::ProcessNewObjects()
{
	if (newObjects.size() > 0)
	{
		for (const auto& o : newObjects)
		{
			o->Awake();
		}
		for (const auto& o : newObjects)
		{
			o->Start();
		}

		objects.insert(objects.end(), newObjects.begin(), newObjects.end());

		drawables.Add(newObjects);
		collidables.Add(newObjects);

		newObjects.clear();
	}
}
void ObjectCollection::ProcessRemovals()
{
	auto objIterator = objects.begin();
	while (objIterator != objects.end())
	{
		auto obj = *objIterator;
		if (obj->IsQueuedForRemoval())
		{
			drawables.ProcessRemovals();
			collidables.ProcessRemovals();
			objIterator = objects.erase(objIterator);
			
		}
		else
		{
			++objIterator;
		}
	}
}