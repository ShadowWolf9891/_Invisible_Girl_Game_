#pragma once
#ifndef Component_h
#define Component_h
#include "window.h"

class Object; //Object requires a reference to Component and Component requires a reference to Object; we forward declare Object.

class Component
{
public:
	using ComponentType = const char*;
	Component(Object* owner) : owner(owner) {};

	virtual void Awake() {};
	virtual void Start() {};

	virtual void Update(float deltaTime) {};
	virtual void LateUpdate(float deltaTime) {};

	Object* owner;
};

#endif /* Component_h */