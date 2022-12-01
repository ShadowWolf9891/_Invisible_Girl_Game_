#pragma once

#ifndef Quest_h
#define Quest_h

#include <SFML/Graphics.hpp>

enum StatusType
{
	UNAVAILABLE = 0,
	AVAILABLE = 1,
	INPROGRESS = 2,
	COMPLETED = 3,
	FINISHED = 4,
	FAILED = 5
};


class Quest 
{
public:

	Quest(int ID, std::string questName): _ID(ID), _name(questName), _status(UNAVAILABLE) {};
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

