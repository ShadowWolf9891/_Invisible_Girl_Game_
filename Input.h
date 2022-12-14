#pragma once
#ifndef Input_h
#define Input_h

#include <SFML/Graphics.hpp>
#include "Bitmask.h"

class Input
{
public:
	enum class Key
	{
		None = 0,
		Left = 1,
		Right = 2,
		Up = 3,
		Down = 4,
		Esc = 5,
		LBracket = 6,
		RBracket = 7,
		E = 8,
		R = 9,
		Shift = 10,
		Tab = 11
	};

	enum class Mouse
	{
		None=0,
		Left = 1,
		Right =2,
		Middle = 3
	};

	void Update();

	bool IsKeyPressed(Key keycode); // Returns true if the key is pressed.
	bool IsKeyDown(Key keycode); // Returns true if the key was just pressed.
	bool IsKeyUp(Key keycode); // Returns true if the key was just released.

	bool IsMousePressed(Mouse keycode); // Returns true if the key is pressed.
	bool IsMouseDown(Mouse keycode); // Returns true if the key was just pressed.
	bool IsMouseUp(Mouse keycode); // Returns true if the key was just released.

	sf::Vector2i GetMousePos();
	void UpdateMousePos();

private:
	Bitmask thisFrameKeys;
	Bitmask lastFrameKeys;

	Bitmask thisFrameMouse;
	Bitmask lastFrameMouse;

	sf::Vector2i mousePosWindow;
};

#endif /* Input_h */