#pragma once

#ifndef Quest_h
#define Quest_h

#include <SFML/Graphics.hpp>

enum StatusType
{
	NOT_ACTIVE = 0,
	ACTIVE = 1,
	COMPLETED = 2
};


class Quest 
{
public:

	Quest(int ID, std::string questName): _ID(ID), _name(questName), _status(NOT_ACTIVE) {};
	virtual ~Quest() {};

	void SetStatus(StatusType status) { _status = status; };
	StatusType GetStatus() { return _status; };
	int GetID() { return _ID; };
	std::string GetName() { return _name; };

private:
	int _ID;
	StatusType _status;
	std::string _name;

};

#endif // Quest_h

