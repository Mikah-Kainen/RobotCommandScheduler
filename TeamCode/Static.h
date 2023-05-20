#pragma once
#include <vector>


enum class Systems : unsigned char
{
	None = 0,

	ContainerIntake = 1,
	MotorA = 1,

	LeftMotor = 2,
	MotorB = 2,

	RightMotor = 4,
	MotorC = 4,

	HoopIntake = 8,
	MotorD = 8,

	Four = 16,
	Five = 32,
	Six = 64,
	Other = 128,

	//Custom flags
	All = 255,
	Chassis = 6,
};

static const int SystemsCount = 8;
static int NextAvailableSchedulerID;

template <typename T>
static bool Contains(std::vector<T> list, T value)
{
	for (T val : list)
	{
		if (val == value)
		{
			return true;
		}
	}
	return false;
}

static std::vector<Systems> GetSystems(unsigned char systemFlags)
{
	std::vector<Systems> returnSystems;
	for (int i = 0; i < SystemsCount; i++)
	{
		unsigned char currentMask = 1 << i;
		if ((systemFlags & currentMask) >> i == 1)
		{
			//Systems currentSystem = (Systems)((unsigned char)pow(2, i));
			Systems currentSystem = (Systems)currentMask;
			returnSystems.push_back(currentSystem);
		}
	}
	return returnSystems;
}


static std::vector<Systems> GetSystems(std::vector<unsigned char> systemFlags)
{
	std::vector<Systems> returnSystems;
	for (unsigned char flag : systemFlags)
	{
		if (!Contains<Systems>(returnSystems, (Systems)flag))
		{
			returnSystems.push_back((Systems)flag);
		}
	}
	return returnSystems;
}

static unsigned char GetRequirementFlag(std::vector<Systems> systems)
{
	unsigned char flag = 0;
	for (Systems system : systems)
	{
		flag |= (unsigned char)system;
	}
	return flag;
}


static unsigned char GetRequirementFlag(std::vector<unsigned char> systems)
{
	unsigned char flag = 0;
	for (unsigned char systemID : systems)
	{
		flag |= systemID;
	}
	return flag;
}
