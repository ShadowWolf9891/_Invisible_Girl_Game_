#pragma once
#ifndef C_Drawable_h
#define C_Drawable_h

#include <SFML/Graphics.hpp>
#include "window.h"

enum class DrawLayer
{
	Default,
	Background,
	Foreground,
	Entities,
	UI
};

class C_Drawable
{
public:
	C_Drawable();
	virtual ~C_Drawable();

	virtual void Draw(Window& window) = 0;
	virtual bool ContinueToDraw() const = 0; //Must be overriden by child classes (C_Sprite.h)

	void SetSortOrder(int order);
	int GetSortOrder() const;

	void SetDrawLayer(DrawLayer drawLayer);
	DrawLayer GetDrawLayer() const;

private:
	int sortOrder;
	DrawLayer layer;
};

#endif /* C_Drawable_h */