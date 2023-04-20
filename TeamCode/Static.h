#pragma once
#include <vector>


static enum class Systems : char
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

static std::vector<Systems> GetSystems(char systemFlags)
{
	std::vector<Systems> returnSystems;
	for (int i = 0; i < SystemsCount; i ++)
	{
		char currentFlag = systemFlags << (SystemsCount - 1);
		int //make these functions work
	}
}

static char CreateFlag(std::vector<Systems> systems)
{

}

