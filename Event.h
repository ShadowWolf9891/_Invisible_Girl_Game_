#pragma once

#ifndef Event_h
#define Event_h

#include <SFML/Graphics.hpp>
#include <string>
#include "Input.h"

//None,
//Idle,
//Walk,
//Run,
//DrawWeapon,
//Sheath,
//Parry,
//Dodge,
//Hurt,
//Dead,
//Crouch,
//Retreat,
//Lunge,
//ShootUp,
//ShootStraight,
//Interact,
//LeftClick,
//RightClick,
//MiddleClick

//Base Event class. All Events dirive from this. Constructor takes a reference to which event was created.
class Event 
{
public:
	using DescriptorType = const char*;
	Event(Event & ref, DescriptorType type, const std::string& name = "") : e(ref), _type(type), _name(name) {};
	inline const DescriptorType getType() const { return _type; };
	inline const std::string& getName() const { return _name; };
	virtual bool IsHandled() { return _handled; };
	virtual void SetHandled(bool _handled) { this->_handled = _handled; };

	inline const int GetTargetID() const { return _targetID; };
	void SetTargetID(const int _targetID) { this->_targetID = _targetID; };

private:
	DescriptorType _type;
	std::string _name;
	bool _handled = false;
	Event& e;
	int _targetID = -1;
};

///////////////Child Classes/////////////////

class E_None : public Event
{	
public:
	E_None() : Event(*this, descriptor, "Default Event") {};
	virtual ~E_None() = default;
	static constexpr DescriptorType descriptor = "E_None";

};

//Movement Events
class E_Idle : public Event
{
public:
	E_Idle() : Event(*this, descriptor, "Begin Idle Event") {};
	virtual ~E_Idle() = default;
	static constexpr DescriptorType descriptor = "E_Idle";

};
class E_Walk : public Event
{
public:
	E_Walk() : Event(*this, descriptor, "Begin Walk Event") {};
	virtual ~E_Walk() = default;
	static constexpr DescriptorType descriptor = "E_Walk";

};
class E_Run : public Event
{
public:
	E_Run() : Event(*this, descriptor, "Begin Run Event") {};
	virtual ~E_Run() = default;
	static constexpr DescriptorType descriptor = "E_Run";
};

// Events
class E_DrawWeapon : public Event
{
public:

	E_DrawWeapon() : Event(*this, descriptor, "Draw Weapon Event") {};
	virtual ~E_DrawWeapon() = default;

	static constexpr DescriptorType descriptor = "E_Draw";
};
class E_Sheath : public Event
{
public:

	E_Sheath() : Event(*this, descriptor, "Sheath Event") {};
	virtual ~E_Sheath() = default;

	static constexpr DescriptorType descriptor = "E_Sheath";
};
class E_Parry : public Event
{
public:

	E_Parry() : Event(*this, descriptor, " Parry Event") {};
	virtual ~E_Parry() = default;

	static constexpr DescriptorType descriptor = "E_Parry";
};
class E_Dodge : public Event
{
public:

	E_Dodge() : Event(*this, descriptor, " Dodge Event") {};
	virtual ~E_Dodge() = default;

	static constexpr DescriptorType descriptor = "E_Dodge";
};
class E_Hurt : public Event
{
public:

	E_Hurt() : Event(*this, descriptor, " Hurt Event") {};
	virtual ~E_Hurt() = default;

	static constexpr DescriptorType descriptor = "E_Hurt";
};
class E_Dead : public Event
{
public:

	E_Dead() : Event(*this, descriptor, " Dead Event") {};
	virtual ~E_Dead() = default;

	static constexpr DescriptorType descriptor = "E_Dead";
};
class E_Crouch : public Event
{
public:

	E_Crouch() : Event(*this, descriptor, " Crouch Event") {};
	virtual ~E_Crouch() = default;

	static constexpr DescriptorType descriptor = "E_Crouch";
};
class E_Retreat : public Event
{
public:

	E_Retreat() : Event(*this, descriptor, " Retreat Event") {};
	virtual ~E_Retreat() = default;

	static constexpr DescriptorType descriptor = "E_Retreat";
};
class E_Lunge : public Event
{
public:

	E_Lunge() : Event(*this, descriptor, " Lunge Event") {};
	virtual ~E_Lunge() = default;

	static constexpr DescriptorType descriptor = "E_Lunge";
};
class E_Attack : public Event
{
public:

	E_Attack() : Event(*this, descriptor, " E_Attack Event") {};
	virtual ~E_Attack() = default;

	static constexpr DescriptorType descriptor = "E_Attack";
};
class E_ShootStraight : public Event
{
public:

	E_ShootStraight() : Event(*this, descriptor, " Shoot Straight Event") {};
	virtual ~E_ShootStraight() = default;

	static constexpr DescriptorType descriptor = "E_ShootStraight";
};

class E_Interact : public Event
{
public:

	E_Interact() : Event(*this, descriptor, "Attemped to Interact with object") {};
	virtual ~E_Interact() = default;

	static constexpr DescriptorType descriptor = "E_Interact";
};

class E_LeftClick : public Event
{
public:

	E_LeftClick() : Event(*this, descriptor, "Left click Event") {};
	virtual ~E_LeftClick() = default;

	static constexpr DescriptorType descriptor = "E_LeftClick";
};

class E_RightClick : public Event
{
public:

	E_RightClick() : Event(*this, descriptor, "Right click Event") {};
	virtual ~E_RightClick() = default;

	static constexpr DescriptorType descriptor = "E_RightClick";
};

class E_MiddleClick : public Event
{
public:

	E_MiddleClick() : Event(*this, descriptor, "Middle click Event") {};
	virtual ~E_MiddleClick() = default;

	static constexpr DescriptorType descriptor = "E_MiddleClick";
};

class E_PauseMovement : public Event
{
public:

	E_PauseMovement() : Event(*this, descriptor, "Pause Movement Event") {};
	virtual ~E_PauseMovement() = default;

	static constexpr DescriptorType descriptor = "E_PauseMovement";
};

class E_UnPauseMovement : public Event
{
public:

	E_UnPauseMovement() : Event(*this, descriptor, "UnPause Movement Event") {};
	virtual ~E_UnPauseMovement() = default;

	static constexpr DescriptorType descriptor = "E_UnPauseMovement";
};

#endif //Event_h


