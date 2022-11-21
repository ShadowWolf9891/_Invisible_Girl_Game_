#include "Dispatcher.h"

//Functions in header

void Dispatcher::subscribe(const Event::DescriptorType& descriptor, SlotType&& funct)
{
	_observers[descriptor].push_back(funct);
}

void Dispatcher::post(const Event& event) const
{
	auto type = event.getType();

	// Ignore events for which we do not have an observer (yet).
	if (_observers.find(type) == _observers.end()) {
		return;
	}
		
	auto&& observers = _observers.at(type);

	for (auto&& observer : observers) {
		observer(event);
	}
		
}
