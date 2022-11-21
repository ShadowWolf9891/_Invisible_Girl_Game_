#pragma once
#include <map>
#include <memory>
#include <string>
template<typename T>
class ResourceAllocator
{
private:
	int currentId = 0;
	std::map<std::string, std::pair<int, std::shared_ptr<T>>> resources;

public:
	int Add(const std::string& filePath)
	{
		auto it = resources.find(filePath); // Store filepath to use as identifier
		if (it != resources.end())
		{
			return it->second.first;
		}
		std::shared_ptr<T> resource = std::make_shared<T>();
		if (!resource->loadFromFile(filePath)) // Any asset we use must have LoadFromFile implemented. SFML resources does by default.
		{
			return -1;
		}
		resources.insert(std::make_pair(filePath, std::make_pair(currentId, resource)));
		return currentId++;
	}
	void Remove(int id) //This iterates over our resource collection and if it finds the resource with the associated id it is removed from the collection.
	{
		for (auto it = resources.begin(); it != resources.end(); ++it)
		{
			if (it->second.first == id)
			{
				resources.erase(it->first);
			}
		}
	}
	std::shared_ptr<T> Get(int id)
	{
		for (auto it = resources.begin(); it != resources.end(); ++it)
		{
			if (it->second.first == id)
			{
				return it->second.second;
			}
		}
		return nullptr;
	}
	bool Has(int id)
	{
		return (Get(id) != nullptr);
	}
};