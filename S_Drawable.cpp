#include "S_Drawable.h"

void S_Drawable::Add(std::vector<std::shared_ptr<Object>>& objects)
{
	for (auto& o : objects)
	{
		Add(o);
	}

	Sort();
}
void S_Drawable::Add(std::shared_ptr<Object> object)
{
	for (auto& objectsDrawable : object->GetDrawables())
	{
		if (objectsDrawable)
		{
			DrawLayer layer = objectsDrawable->GetDrawLayer();

			auto itr = drawables.find(layer);
			if (itr != drawables.end())
			{
				drawables[layer].push_back(objectsDrawable);
			}
			else
			{
				std::vector<std::shared_ptr<C_Drawable>> objs;
				objs.push_back(objectsDrawable);

				drawables.insert(std::make_pair(layer, objs));
			}
		}
	}
}
void S_Drawable::Sort()
{
	for (auto& layer : drawables)
	{
		if (!std::is_sorted(layer.second.begin(), layer.second.end(), LayerSort))
		{
			std::sort(layer.second.begin(), layer.second.end(), LayerSort);
		}

	}
}
bool S_Drawable::LayerSort(std::shared_ptr<C_Drawable> a, std::shared_ptr<C_Drawable> b)
{
	return a->GetSortOrder() < b->GetSortOrder();
}

void S_Drawable::Draw(Window& window)
{
	for (auto& layer : drawables)
	{
		for (auto& drawable : layer.second)
		{
			drawable->Draw(window);
		}
	}
}
void S_Drawable::ProcessRemovals()
{
	for (auto& layer : drawables)
	{
		auto objIterator = layer.second.begin();
		while (objIterator != layer.second.end())
		{
			auto obj = *objIterator;
			if (!obj->ContinueToDraw())
			{
				objIterator = layer.second.erase(objIterator);	
			}
			else
			{
				++objIterator;
			}
		}
	}
}