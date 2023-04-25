#pragma once
#include <vector>


static enum class Systems : unsigned char
{
	None = 0,
	MotorA = 1,
	MotorB = 2,
	MotorC = 4,
	Three = 8,
	Four = 16,
	Five = 32,
	Six = 64,
	Other = 128, //Other should always be last System because it is used to track the length of Systems
	All = 255,
};

static const int SystemsCount = 8;

//Untested
static std::vector<Systems> GetSystems(unsigned char systemFlags)
{
	std::vector<Systems> returnSystems;
	for (int i = 0; i < SystemsCount; i ++)
	{
		unsigned char currentFlag = systemFlags << (i - 1);
		currentFlag = currentFlag >> (i - 1);
		returnSystems.push_back((Systems)currentFlag);
	}
	return returnSystems;
}

//Untested
static std::vector<Systems> GetSystems(std::vector<unsigned char> systemFlags)
{
	std::vector<Systems> returnSystems;
	for (unsigned char flag : systemFlags)
	{
		returnSystems.push_back((Systems)flag);
	}
	return returnSystems;
}

//Untested
static unsigned char CreateFlag(std::vector<Systems> systems)
{
	unsigned char flag = 0;
	for (Systems system : systems)
	{
		flag |= (unsigned char)system;
	}
	return flag;
}

