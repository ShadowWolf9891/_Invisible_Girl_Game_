#pragma once

#ifndef C_Events_h
#define C_Events_h

#include <stack>

#include "Event.h"
#include "Component.h"

class C_Events : public Component
{

public:

	C_Events(Object* owner) : Component(owner)
	{

	};
	virtual ~C_Events() = default;
	

	void Update(float deltaTime) override;

	void PushEvent(Event e);
	Event PopEvent();
	Event PeekEvent();
	void HandleNextEvent();
	bool IsEmpty();


	std::shared_ptr<std::stack<Event>> GetStack();

private:
	std::stack<Event> eventStack;
};


#endif // C_Events_h
