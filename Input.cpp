#include "Input.h"

void Input::Update()
{ 
	// Stores the current framekeyes as the last framekeys
	lastFrameKeys.SetMask(thisFrameKeys);
	lastFrameMouse.SetMask(thisFrameMouse);

	//Sets or clears a bit depending on if the key is pressed or not
	thisFrameKeys.SetBit((int)Key::Left,(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||(sf::Keyboard::isKeyPressed(sf::Keyboard::A))); 
	thisFrameKeys.SetBit((int)Key::Right,(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ||(sf::Keyboard::isKeyPressed(sf::Keyboard::D)));
	thisFrameKeys.SetBit((int)Key::Up,(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ||(sf::Keyboard::isKeyPressed(sf::Keyboard::W)));
	thisFrameKeys.SetBit((int)Key::Down,(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) ||(sf::Keyboard::isKeyPressed(sf::Keyboard::S)));
	thisFrameKeys.SetBit((int)Key::Esc,sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));
	thisFrameKeys.SetBit((int)Key::LBracket,sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket));
	thisFrameKeys.SetBit((int)Key::RBracket,sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket));
	thisFrameKeys.SetBit((int)Key::E, sf::Keyboard::isKeyPressed(sf::Keyboard::E));
	thisFrameKeys.SetBit((int)Key::R, sf::Keyboard::isKeyPressed(sf::Keyboard::R));
	thisFrameKeys.SetBit((int)Key::Shift, sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));
	thisFrameKeys.SetBit((int)Key::Tab, sf::Keyboard::isKeyPressed(sf::Keyboard::Tab));

	thisFrameMouse.SetBit((int)Mouse::Left, sf::Mouse::isButtonPressed(sf::Mouse::Left));
	thisFrameMouse.SetBit((int)Mouse::Right, sf::Mouse::isButtonPressed(sf::Mouse::Right));
	thisFrameMouse.SetBit((int)Mouse::Middle, sf::Mouse::isButtonPressed(sf::Mouse::Middle));

	UpdateMousePos();
}
// Return true if the specified key is currently being pressed.
bool Input::IsKeyPressed(Key keycode)
{
	return thisFrameKeys.GetBit((int)keycode);
}
// Returns true if the key was just pressed 
// (i.e. registered as pressed this frame but not the previous).
bool Input::IsKeyDown(Key keycode)
{
	bool lastFrame = lastFrameKeys.GetBit((int)keycode);
	bool thisFrame = thisFrameKeys.GetBit((int)keycode);
	return thisFrame && !lastFrame;
}
// Returns true if the key was just released (i.e. registered as 
// pressed last frame but not the current frame).
bool Input::IsKeyUp(Key keycode)
{
	bool lastFrame = lastFrameKeys.GetBit((int)keycode);
	bool thisFrame = thisFrameKeys.GetBit((int)keycode);
	return !thisFrame && lastFrame;
}

bool Input::IsMousePressed(Mouse keycode)
{
	return thisFrameMouse.GetBit((int)keycode);
}

bool Input::IsMouseDown(Mouse keycode)
{
	bool lastFrame = lastFrameMouse.GetBit((int)keycode);
	bool thisFrame = thisFrameMouse.GetBit((int)keycode);
	return thisFrame && !lastFrame;
}

bool Input::IsMouseUp(Mouse keycode)
{
	bool lastFrame = lastFrameMouse.GetBit((int)keycode);
	bool thisFrame = thisFrameMouse.GetBit((int)keycode);
	return !thisFrame && lastFrame;
}

sf::Vector2i Input::GetMousePos()
{
	return mousePosWindow;
}

//Update mouse position relative to window (Vector2i)
void Input::UpdateMousePos()
{
    mousePosWindow = sf::Mouse::getPosition();
}