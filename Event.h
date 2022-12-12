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
//DrawBow,
//SheathBow,
//BowParry,
//BowDodge,
//BowHurt,
//BowDead,
//BowIdle,
//BowMove,
//BowCrouch,
//BowRetreat,
//BowLunge,
//BowShootUp,
//BowShootStraight
//Interact

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

//Bow Events
class E_BowDraw : public Event
{
public:

	E_BowDraw() : Event(*this, descriptor, "Draw Bow Event") {};
	virtual ~E_BowDraw() = default;

	static constexpr DescriptorType descriptor = "E_BowDraw";
};
class E_BowSheath : public Event
{
public:

	E_BowSheath() : Event(*this, descriptor, "Sheath Bow Event") {};
	virtual ~E_BowSheath() = default;

	static constexpr DescriptorType descriptor = "E_BowSheath";
};
class E_BowParry : public Event
{
public:

	E_BowParry() : Event(*this, descriptor, "Bow Parry Event") {};
	virtual ~E_BowParry() = default;

	static constexpr DescriptorType descriptor = "E_BowParry";
};
class E_BowDodge : public Event
{
public:

	E_BowDodge() : Event(*this, descriptor, "Bow Dodge Event") {};
	virtual ~E_BowDodge() = default;

	static constexpr DescriptorType descriptor = "E_BowDodge";
};
class E_BowHurt : public Event
{
public:

	E_BowHurt() : Event(*this, descriptor, "Bow Hurt Event") {};
	virtual ~E_BowHurt() = default;

	static constexpr DescriptorType descriptor = "E_BowHurt";
};
class E_BowDead : public Event
{
public:

	E_BowDead() : Event(*this, descriptor, "Bow Dead Event") {};
	virtual ~E_BowDead() = default;

	static constexpr DescriptorType descriptor = "E_BowDead";
};
class E_BowIdle : public Event
{
public:

	E_BowIdle() : Event(*this, descriptor, "Bow Idle Event") {};
	virtual ~E_BowIdle() = default;

	static constexpr DescriptorType descriptor = "E_BowIdle";
};
class E_BowMove : public Event
{
public:

	E_BowMove() : Event(*this, descriptor, "Bow Move Event") {};
	virtual ~E_BowMove() = default;

	static constexpr DescriptorType descriptor = "E_BowMove";
};
class E_BowCrouch : public Event
{
public:

	E_BowCrouch() : Event(*this, descriptor, "Bow Crouch Event") {};
	virtual ~E_BowCrouch() = default;

	static constexpr DescriptorType descriptor = "E_BowCrouch";
};
class E_BowRetreat : public Event
{
public:

	E_BowRetreat() : Event(*this, descriptor, "Bow Retreat Event") {};
	virtual ~E_BowRetreat() = default;

	static constexpr DescriptorType descriptor = "E_BowRetreat";
};
class E_BowLunge : public Event
{
public:

	E_BowLunge() : Event(*this, descriptor, "Bow Lunge Event") {};
	virtual ~E_BowLunge() = default;

	static constexpr DescriptorType descriptor = "E_BowLunge";
};
class E_BowShootUp : public Event
{
public:

	E_BowShootUp() : Event(*this, descriptor, "Bow Shoot Up Event") {};
	virtual ~E_BowShootUp() = default;

	static constexpr DescriptorType descriptor = "E_BowShootUp";
};
class E_BowShootStraight : public Event
{
public:

	E_BowShootStraight() : Event(*this, descriptor, "Bow Shoot Straight Event") {};
	virtual ~E_BowShootStraight() = default;

	static constexpr DescriptorType descriptor = "E_BowShootStraight";
};

class E_Interact : public Event
{
public:

	E_Interact() : Event(*this, descriptor, "Attemped to Interact with object") {};
	virtual ~E_Interact() = default;

	static constexpr DescriptorType descriptor = "E_Interact";
};


#endif //Event_h


