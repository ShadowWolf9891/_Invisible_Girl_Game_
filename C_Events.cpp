#include "C_Events.h"
#include "Object.h"


void C_Events::Update(float deltaTime)
{
	HandleNextEvent();
}

void C_Events::PushEvent(Event e)
{
	eventStack.push(e);
}

Event C_Events::PopEvent()
{
	Event e = PeekEvent();
	eventStack.pop();

	return e;
}

Event C_Events::PeekEvent()
{
	if (!IsEmpty()) {
		return eventStack.top();
	}

	Event e = Event(e, "E_None", "Default Event");

	return e;
}

void C_Events::HandleNextEvent()
{
	if (!eventStack.empty()) 
	{
		Event e = PeekEvent();

		if (!e.IsHandled()) 
		{
			owner->context->dispatcher->post(e);
			e.SetHandled(true);
		}
		
	}
}

bool C_Events::IsEmpty() 
{
	return eventStack.empty();
}

std::shared_ptr<std::stack<Event>> C_Events::GetStack()
{
	return std::make_shared<std::stack<Event>>(eventStack);
}
