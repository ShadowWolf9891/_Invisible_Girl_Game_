#pragma once
#ifndef C_InstanceID_h
#define C_InstanceID_h
#include "Component.h"

class C_InstanceID : public Component
{
public:
	C_InstanceID(Object* owner) : Component(owner), id(count++)
	{

	};
	virtual ~C_InstanceID() = default;
	
	unsigned int Get() const;

private:
	static unsigned int count;
	unsigned int id;
};

#endif /* C_InstanceID_h */