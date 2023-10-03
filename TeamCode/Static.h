#pragma once
#include <vector>
#include <iostream>
#include <exception>
#include <functional>


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
	Chassis = LeftMotor + RightMotor,
	Robot = Chassis + ContainerIntake + HoopIntake,
};

static const unsigned int SystemsCount = 8;
static unsigned int NextAvailableSchedulerID = 1; //Starts at 1 so that zero can be used to represent no unlockKeys for scheduleables

static std::function<bool()> ReturnTrue = std::function<bool()>([&]() {return true; });

static std::function<bool()> ThrowError = std::function<bool()>([&]()
	{
		//throw std::exception("Saving User from Wierd Error Message");
		std::cout << "~~~!ERROR!~~~!ERROR!~~~!ERROR!~~~" << std::endl;
		while (true) {};
		return false;
	});

static std::function<bool()> NoFunctionProvided = std::function<bool()>([&]()
	{
		//throw std::exception("No Function Provided");
		std::cout << "NOFUNCTIONPROVIDED" << std::endl;
		while (true);
		return false;
	});

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

template <typename T>
static void Pop_Front(std::vector<T>& list)
{
	std::vector<T> newList = std::vector<T>();
	for (int i = 1; i < list.size(); i++)
	{
		newList.push_back(list[i]);
	}
	list = newList;
}

static std::vector<Systems> GetSystems(unsigned char systemFlags)
{
	std::vector<Systems> returnSystems;
	if (systemFlags == 0)
	{
		returnSystems.push_back(Systems::None);
	}
	else
	{
		for (unsigned int i = 0; i < SystemsCount; i++)
		{
			unsigned char currentMask = 1 << i;
			if ((systemFlags & currentMask) >> i == 1)
			{
				//Systems currentSystem = (Systems)((unsigned char)pow(2, i));
				Systems currentSystem = (Systems)currentMask;
				returnSystems.push_back(currentSystem);
			}
		}
	}
	return returnSystems;
}


static std::vector<Systems> GetSystems(std::vector<unsigned char> systemFlags)
{
	std::vector<Systems> returnSystems;
	if (systemFlags.size() == 0)
	{
		returnSystems.push_back(Systems::None);
	}
	else
	{
		for (unsigned char flag : systemFlags)
		{
			if (!Contains<Systems>(returnSystems, (Systems)flag))
			{
				returnSystems.push_back((Systems)flag);
			}
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

static bool IsSubset(unsigned char flag, unsigned char set)
{
	return (flag & set) == flag;
}