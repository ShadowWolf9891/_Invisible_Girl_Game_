#pragma once
#ifndef Object_h
#define Object_h

#include <vector>

#include "window.h"
//#include <iostream>
#include "Component.h"
#include "C_Transform.h"
#include "C_Drawable.h"
#include "C_InstanceID.h"
#include "SharedContext.h"
#include "C_Collidable.h"
#include "C_Tag.h"

//TODO: look into replacing object hierarchy with std::function - https://probablydance.com/2012/12/16/the-importance-of-stdfunction/
class Object
{
public:
	
	Object(SharedContext* context);

	void Awake(); // Awake is called when object created. Use to ensure required components are present.
	void Start(); // Start is called after Awake method. Use to initialise variables.

	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void Draw(Window& window);

	bool IsQueuedForRemoval();
	void QueueForRemoval();

	void OnCollisionEnter(std::shared_ptr<C_BoxCollider> other);
	void OnCollisionStay(std::shared_ptr<C_BoxCollider> other);
	void OnCollisionExit(std::shared_ptr<C_BoxCollider> other);

	//Add component implemented in header
	template <typename T> std::shared_ptr<T> AddComponent() //Ensures we only add a class that derives from component. Tested at compile time
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
		// Check that we don't already have a component of this type.
		for (auto& exisitingComponent : components)
		{
			// Currently we prevent adding the same component twice. This may be something we will change in future.
			if (std::dynamic_pointer_cast<T>(exisitingComponent))
			{
				return std::dynamic_pointer_cast<T>(exisitingComponent); // Dynamic casting casts a superclass to a subclass
			}
		}
		// The object does not have this component so we create it and add it to our list.
		std::shared_ptr<T> newComponent = std::make_shared<T>(this);
		components.push_back(newComponent);

		if (std::dynamic_pointer_cast<C_Drawable>(newComponent))
		{
			drawable = std::dynamic_pointer_cast<C_Drawable>(newComponent);
		}
		// Check if the component inherits from C_Collidable. If it does store it in a separate vector.
		if (std::dynamic_pointer_cast<C_Collidable>(newComponent))
		{
			collidables.push_back(std::dynamic_pointer_cast<C_Collidable>(newComponent));
		}
		return newComponent;
	};
	template <typename T> std::shared_ptr<T> GetComponent()
	{
			for (auto& exisitingComponent : components)
			{
				if (std::dynamic_pointer_cast<T>(exisitingComponent))
				{
					return std::dynamic_pointer_cast<T>(exisitingComponent);
				}
			}
		return nullptr;
	};

	template <typename T> std::vector<std::shared_ptr<T>> GetComponents()
	{
		std::vector<std::shared_ptr<T>> matchingComponents;
		for (auto& exisitingComponent : components)
		{
			if (std::dynamic_pointer_cast<T>(exisitingComponent))
			{
				matchingComponents.emplace_back(std::dynamic_pointer_cast<T>(exisitingComponent));
			}
		}

		return matchingComponents;
	};

	std::shared_ptr<C_Drawable> GetDrawable();

	std::shared_ptr<C_Transform> transform; //Adds a Transform component to all objects. Could make this private and have get/set subroutines
	std::shared_ptr<C_InstanceID> instanceID;
	std::shared_ptr<C_Tag> tag;

	SharedContext* context; //TODO: Make mutator

	void SetIsOnGround(bool isOnGround) { this->isOnGround = isOnGround; };
	bool GetIsOnGround() { return this->isOnGround; };

	void SetName(std::string _name) { this->name = _name; };
	std::string GetName() { return this->name; };

private:

	std::string name;

	std::vector<std::shared_ptr<Component>> components;
	std::shared_ptr<C_Drawable> drawable;
	std::vector<std::shared_ptr<C_Collidable>> collidables;
	bool queuedForRemoval;
	bool isOnGround;
};
#endif /* Object_h */