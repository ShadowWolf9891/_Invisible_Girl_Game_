#include "S_Collidable.h"
S_Collidable::S_Collidable(Quadtree& collisionTree) : collisionTree(collisionTree)
{
	Bitmask defaultCollisions; // Create a bitmask to store the collision data.
	defaultCollisions.SetBit((int)CollisionLayer::Default);   // We set the bit for each layer that this layer will collide with.
	collisionLayers.insert(std::make_pair(CollisionLayer::Default, defaultCollisions)); // Add this value to our collisionLayer map
	collisionLayers.insert(std::make_pair(CollisionLayer::Tile, Bitmask(0))); //No collision for tile layers

	Bitmask playerCollisions;
	playerCollisions.SetBit((int)CollisionLayer::Default);
	playerCollisions.SetBit((int)CollisionLayer::Tile);
	playerCollisions.SetBit((int)CollisionLayer::NPC);
	collisionLayers.insert(std::make_pair(CollisionLayer::Player, playerCollisions));

	Bitmask projectileCollisions;
	projectileCollisions.SetBit((int)CollisionLayer::Tile);
	projectileCollisions.SetBit((int)CollisionLayer::NPC);
	collisionLayers.insert(std::make_pair(CollisionLayer::Projectile, projectileCollisions));

	Bitmask npcCollisions;
	npcCollisions.SetBit((int)CollisionLayer::Tile);
	collisionLayers.insert(std::make_pair(CollisionLayer::NPC, npcCollisions));
}
//Add vector of objects and check if they are collidable
void S_Collidable::Add(std::vector<std::shared_ptr<Object>>& objects)
{
	for (auto o : objects)
	{
		auto collider = o->GetComponent<C_BoxCollider>();
		if (collider)
		{
			CollisionLayer layer = collider->GetLayer();

			auto itr = collidables.find(layer);
			if (itr != collidables.end())
			{
				collidables[layer].push_back(collider); //Map of collidable objects on each layer
			}
			else //If it is the first object on a layer
			{
				collidables.insert(std::make_pair(layer, std::vector<std::shared_ptr<C_BoxCollider>>{collider}));
			}
		}
	}
}
//Loop through all collidable objects and remove any queued for removal
void S_Collidable::ProcessRemovals()
{
	for (auto& layer : collidables)
	{
		auto itr = layer.second.begin();
		while (itr != layer.second.end())
		{
			if ((*itr)->owner->IsQueuedForRemoval())
			{
				ProcessCollidingObjects(); //Added call to ProcessCollidingObjects to fix error of trying to remove C_boxCollider after already removing object it is attached to
				itr = layer.second.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
}
void S_Collidable::UpdatePositions(std::vector<std::shared_ptr<Object>>& objects)
{
	for (auto o : objects)
	{
		if (!o->transform->isStatic())
		{
			auto collider = o->GetComponent<C_BoxCollider>();

			if (collider)
			{
				collisionTree.UpdatePosition(collider);
			}
		}
	}
}
void S_Collidable::Update()
{
	collisionTree.DrawDebug();
	collisionTree.Clear(); //Clears the quadtree
	for (auto maps = collidables.begin(); maps != collidables.end(); ++maps)
	{
		for (auto& collidable : maps->second)
		{
			collisionTree.Insert(collidable); //Reinsert the collidables. Replace with dynamic node updates if needed for performance
		}
	}
	ProcessCollidingObjects();
	Resolve();
}
void S_Collidable::Resolve()
{
	for (auto maps = collidables.begin(); maps != collidables.end(); ++maps) //Loop through every layer stored in system
	{
		// If this layer collides with nothing then no need to perform any further checks.
		if (collisionLayers[maps->first].GetMask() == 0)
		{
			continue;
		}
		for (auto& collidable : maps->second) ////Loop through every collidable stored in layer
		{
			// If this collidable is static then no need to check if it's colliding with other objects.
			if (collidable->owner->transform->isStatic())
			{
				continue;
			}
			std::vector<std::shared_ptr<C_BoxCollider>> collisions = collisionTree.Search(collidable->GetCollidable()); // We pass the collidables area to the quadtree and retrieve a vector of objects that intersect with that area.
			for (auto& collision : collisions) //For every object we are colliding with
			{
				// Make sure we do not resolve collisions between the same object.
				if (collidable->owner->instanceID->Get() == collision->owner->instanceID->Get())
				{
					continue;
				}
				bool layersCollide = collisionLayers[collidable->GetLayer()].GetBit(((int)collision->GetLayer()));
				if (layersCollide) //We check if the two layers collide. If not then we can skip resolution.
				{
					Manifold m = collidable->Intersects(collision); //The collision is passed to the collidable component of the initial object
					if (m.colliding)
					{
						
						
						auto collisionPair = objectsColliding.emplace(std::make_pair(collidable, collision)); //collisionPair is a pair with the second item as a bool of if the insertion was successful
						if (collisionPair.second) //Insertion is only successful if not already checking collision in hash table
						{
							collidable->owner->OnCollisionEnter(collision);
							collision->owner->OnCollisionEnter(collidable);
						}

						Debug::DrawRect(collision->GetCollidable(), sf::Color::Red);
						Debug::DrawRect(collidable->GetCollidable(), sf::Color::Red);

						if (collision->owner->transform->isStatic())
						{
							collidable->ResolveOverlap(m);
						}
						else
						{

							//TODO: how shall we handle collisions when both objects are not static?
							// We could implement rigidbodies and mass.
							collidable->ResolveOverlap(m);
						}
					}
				}
			}
		}
	}
}
void S_Collidable::ProcessCollidingObjects()
{
	auto itr = objectsColliding.begin();
	while (itr != objectsColliding.end())
	{
		auto pair = *itr;

		std::shared_ptr<C_BoxCollider> first = pair.first;
		std::shared_ptr<C_BoxCollider> second = pair.second;

		if (first->owner->IsQueuedForRemoval() || second->owner->IsQueuedForRemoval())
		{
			first->owner->OnCollisionExit(second);
			second->owner->OnCollisionExit(first);

			itr = objectsColliding.erase(itr);
		}
		else
		{
			Manifold m = first->Intersects(second);

			if (!m.colliding)
			{
				first->owner->OnCollisionExit(second);
				second->owner->OnCollisionExit(first);

				itr = objectsColliding.erase(itr);
			}
			else
			{
				first->owner->OnCollisionStay(second);
				second->owner->OnCollisionStay(first);

				++itr;
			}
		}
	}
}