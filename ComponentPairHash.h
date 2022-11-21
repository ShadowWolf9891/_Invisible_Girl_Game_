#pragma once
#ifndef ComponentPairHash_h
#define ComponentPairHash_h

#include <cstddef>

struct ComponentPairHash
{
	template <typename T>
	std::size_t operator()(T t) const
	{
		std::size_t x = t.first->owner->instanceID->Get();
		std::size_t y = t.second->owner->instanceID->Get();
		return (x >= y) ? (x * x + x + y) : (y * y + y + x);
	}
};
#endif /* ComponentPairHash_h */