#pragma once
#ifndef Dispatcher_h
#define Dispatcher_h

#include <functional>
#include <map>
#include <vector>
#include <algorithm>

#include "Event.h"



class Dispatcher
{
public:

	using SlotType = std::function<void(const Event&)>;
	
	//Subscribes an observer
	void subscribe(const Event::DescriptorType& descriptor, SlotType&& funct);

	void post(const Event& event) const;
private:

	std::map<Event::DescriptorType, std::vector<SlotType> > _observers;
	
};

#endif //Dispatcher_h