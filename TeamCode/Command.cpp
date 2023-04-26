#pragma once
#include <functional>
#include <exception>
#include "FunctionManager.h"

//#include "Scheduler.h"
//these two are included in Scheduler.h
//#include "FunctionManager.h"
//#include "Static.h"

//class ScheduleableCommand : FunctionManager::Scheduleable
//{
//public:
//	ScheduleableCommand(std::function<bool()> backingFunction, unsigned char requirementFlags)
//		:FunctionManager::Scheduleable(backingFunction, requirementFlags) {}
//};

class ScheduledCommand : public FunctionManager::Scheduleable
{

public:

	ScheduledCommand(std::function<bool()> backingFunction, unsigned char requirementFlags)
		: FunctionManager::Scheduleable(backingFunction, requirementFlags) {}
};

//Link about Parameter Packs: https://en.cppreference.com/w/cpp/language/parameter_pack
template <typename... Ts>
class Command
{
private:
	std::function<bool(Ts...)> backingFunction;
	unsigned char requirementFlags;

public:

	Command(std::function<bool(Ts...)> backingFunction, unsigned char requirementFlags)
		:backingFunction{ backingFunction }, requirementFlags{ requirementFlags } {}

	//Command(std::function<bool(Ts...)> backingFunction, std::vector<Systems> requiredSystems)
	//	: Command(backingFunction, CreateFlag(requiredSystems)) {}

	//Command(std::function<bool(Ts...)> backingFunction, Systems system)
	//	: Command(backingFunction, system) {}


	//Use for testing purposes if needed
	//bool Activate(Ts... params)
	//{
	//	return backingFunction(params...);
	//}

	//This function isn't bad but at the same time I don't really see a good use case and it's not so good if commands without requirements exist so I'll force the constructor to take the requirements
	//void AddRequirement(Systems requirement)
	//{
	//	requirementFlags |= requirement;
	//}

	//Maybe more complicated commands can have things like end behavior added to them with functions


	//Make this work
	//Scheduleable GetScheduleable(Ts... params)
	//{
	//}

	void Schedule(Ts... params)
	{
		//Schedule a ScheduleableCommand from GetScheduleable
		throw std::exception("Function incomplete. No way to access the scheduler. Fix this Mikah!");
	}


};
