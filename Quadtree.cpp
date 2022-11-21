#include "Quadtree.h"



//maxObjects: how many objects a node can contain before it splits into child nodes.
//maxLevels : starting from the base node(0) how many times can it(and its children) split.
//level : this is the current level of the tree. This is set to 0 in the default constructor as it is the base node.
//bounds : the area of the quadtree.This controls the positionand size of the tree.
//parent : if this is a child node then parent node is included here, otherwise this will be a nulptr.

Quadtree::Quadtree() : Quadtree(5, 5, 0, {sf::Vector2f( - 8.f, -8.f), sf::Vector2f(2048, 2048)}, nullptr)
{

}
Quadtree::Quadtree(int maxObjects, int maxLevels, int level, sf::FloatRect bounds, Quadtree* parent) 
	: maxObjects(maxObjects), maxLevels(maxLevels), level(level), bounds(bounds), parent(parent)
{

}

void Quadtree::DrawDebug()
{
	if (children[0] != nullptr)
	{
		for (int i = 0; i < 4; i++)
		{
			children[i]->DrawDebug();
		}
	}
	Debug::DrawRect(bounds, sf::Color::Red);
}

void Quadtree::Insert(std::shared_ptr<C_BoxCollider> object)
{
	if (!bounds.intersects(object->GetCollidable()))
	{
		return;
	}

	if (children[0] != nullptr) // Check if it has any children nodes. If it finds 1 assume the other 3 are there too
	{
		int indexToPlaceObject = GetChildIndexForObject(object->GetCollidable()); //return the index of the node that the object should belong to.

		if (indexToPlaceObject != thisTree) // If returns child node index, call child insert function and return
		{
			children[indexToPlaceObject]->Insert(object);
			return;
		}
	}

	objects.emplace_back(object); // This node has no child nodes so we add object to this nodes collection

	if (objects.size() > maxObjects && level < maxLevels && children[0] == nullptr) // Check if we exceded max number of objects per node
	{
		Split(); // If we exceed max number, initialize child nodes.

		auto objIterator = objects.begin(); // Iterate over all objects in this node and decide if it should be in a child node instead
		while (objIterator != objects.end())
		{
			auto obj = *objIterator;
			int indexToPlaceObject = GetChildIndexForObject(obj->GetCollidable());
			if (indexToPlaceObject != thisTree)
			{
				children[indexToPlaceObject]->Insert(obj);
				objIterator = objects.erase(objIterator);
			}
			else
			{
				++objIterator;
			}
		}
	}
}
void Quadtree::Remove(std::shared_ptr<C_BoxCollider> object)
{
	int index = GetChildIndexForObject(object->GetCollidable());
	if (index == thisTree || children[index] == nullptr) //Check if object belongs to this node
	{
		for (int i = 0; i < objects.size(); i++) //Loop through all objects in this node
		{
			if (objects.at(i)->owner->instanceID->Get() == object->owner->instanceID->Get())
			{
				objects.erase(objects.begin() + i); //Remove object
				break;
			}
		}
	}
	else
	{
		return children[index]->Remove(object); //Call remove on child node if object belongs to child
	}
}

//Clear all objects in this node, then do the same for all child nodes
void Quadtree::Clear()
{
	objects.clear(); 

	for (int i = 0; i < 4; i++)
	{
		if (children[i] != nullptr)
		{
			children[i]->Clear();
			children[i] = nullptr;
		}
	}
}
void Quadtree::UpdatePosition(std::shared_ptr<C_BoxCollider> object)
{
	Quadtree* quadTree = this;

	const sf::FloatRect& prevObjectRect = object->GetPreviousFrameCollidable();

	int index = quadTree->GetChildIndexForObject(prevObjectRect);

	while (index != thisTree && quadTree->children[index] != nullptr)
	{
		quadTree = quadTree->children[index].get();
		index = quadTree->GetChildIndexForObject(prevObjectRect);
	}

	for (int i = 0; i < quadTree->objects.size(); i++)
	{
		if (quadTree->objects.at(i)->owner->instanceID->Get() == object->owner->instanceID->Get())
		{
			const sf::FloatRect& objectRect = object->GetCollidable();

			if (!quadTree->GetBounds().intersects(objectRect))
			{
				quadTree->Remove(object);

				quadTree = quadTree->parent;
				while (quadTree != nullptr && !quadTree->GetBounds().intersects(objectRect))
				{
					quadTree = quadTree->parent;
				}

				if (quadTree != nullptr)
				{
					quadTree->Insert(object);
				}
			}

			break;
		}
	}
}
//Public Search function
std::vector<std::shared_ptr<C_BoxCollider>> Quadtree::Search(const sf::FloatRect& area)
{
	std::vector<std::shared_ptr<C_BoxCollider>> possibleOverlaps; // Create vector to store objects that may overlap search area
	Search(area, possibleOverlaps); //Call private search function passing in vector as reference

	std::vector<std::shared_ptr<C_BoxCollider>> returnList; 

	for (auto& collider : possibleOverlaps)
	{
		if (area.intersects(collider->GetCollidable())) //Check if area actually intersects the object's collider
		{
			returnList.emplace_back(collider); //If the object is intersecting, add it to return list
		}
	}
	return returnList;
}
//Private Search function
void Quadtree::Search(const sf::FloatRect& area, std::vector<std::shared_ptr<C_BoxCollider>>& overlappingObjects)
{	
	//Add all objects in this node to vector, either objects that dont fit into child nodes, or all objects in this node if there are no child nodes
	for (auto & o : objects) 
	{
		if (!o->owner->IsQueuedForRemoval()) {
			overlappingObjects.insert(overlappingObjects.end(), o); 
		}
	}
	

	if (children[0] != nullptr)
	{
		int index = GetChildIndexForObject(area); //Get child index for search area
		if (index == thisTree) //If it returns this nodes index, and we have children nodes, then search them too.
		{
			for (int i = 0; i < 4; i++)
			{
				if (children[i]->GetBounds().intersects(area))
				{
					children[i]->Search(area, overlappingObjects);
				}
			}
		}
		else //If it returns and index to child node, then call that child node.
		{
			children[index]->Search(area, overlappingObjects);
		}
	}
}
const sf::FloatRect& Quadtree::GetBounds() const
{
	return bounds;
}
int Quadtree::GetChildIndexForObject(const sf::FloatRect& objectBounds)
{
	//GetChildIndexForObject performs four checks to see if the provided area is totally contained within the north, south, east, or west 
	//of this nodes bounds.It then uses those checks to see if the area will fit within a child nodes bounds.

	int index = -1;
	double verticalDividingLine = bounds.left + bounds.width * 0.5f;
	double horizontalDividingLine = bounds.top + bounds.height * 0.5f;
	bool north = objectBounds.top < horizontalDividingLine && (objectBounds.height + objectBounds.top < horizontalDividingLine);
	bool south = objectBounds.top > horizontalDividingLine;
	bool west = objectBounds.left < verticalDividingLine && (objectBounds.left + objectBounds.width < verticalDividingLine);
	bool east = objectBounds.left > verticalDividingLine;
	if (east)
	{
		if (north)
		{
			index = childNE;
		}
		else if (south)
		{
			index = childSE;
		}
	}
	else if (west)
	{
		if (north)
		{
			index = childNW;
		}
		else if (south)
		{
			index = childSW;
		}
	}
	return index;
}
//Split current node into 4 child nodes and add them to quadtree array of children
void Quadtree::Split()
{
	const int childWidth = bounds.width / 2;
	const int childHeight = bounds.height / 2;
	//TODO: Instatiate using loop
	children[childNE] = std::make_shared<Quadtree>(maxObjects, maxLevels, level + 1,
		sf::FloatRect(sf::Vector2f(bounds.left + childWidth, bounds.top), sf::Vector2f(childWidth, childHeight)),this);
	children[childNW] = std::make_shared<Quadtree>(maxObjects, maxLevels, level + 1,
		sf::FloatRect(sf::Vector2f(bounds.left, bounds.top), sf::Vector2f(childWidth, childHeight)),this);
	children[childSW] = std::make_shared<Quadtree>(maxObjects, maxLevels, level + 1,
		sf::FloatRect(sf::Vector2f(bounds.left, bounds.top + childHeight), sf::Vector2f(childWidth, childHeight)),this);
	children[childSE] = std::make_shared<Quadtree>(maxObjects, maxLevels, level + 1,
		sf::FloatRect(sf::Vector2f(bounds.left + childWidth, bounds.top + childHeight), sf::Vector2f(childWidth, childHeight)),this);
}
