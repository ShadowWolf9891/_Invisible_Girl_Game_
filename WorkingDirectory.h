#pragma once
#ifndef RESOURCE_PATH_HPP
#define RESOURCE_PATH_HPP
#include <string>
#ifdef MACOS 
#include "CoreFoundation/CoreFoundation.h"
#endif
class WorkingDirectory
{
public:
	WorkingDirectory();
	inline const std::string& Get() //returns a string that points to current directory
	{
		return path;
	}
private:
	std::string path;
};
#endif