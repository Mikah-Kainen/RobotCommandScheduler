#pragma once
#include <functional>
#include <exception>
#include <iostream>
//#include "../../inc/SchedulerInc/FunctionManager.h"
//#include "../../inc/SchedulerInc/Static.h"
#include "FunctionManager.h"
#include "Static.h"
#include "MemoryEater.cpp"

//class ScheduleableCommand : FunctionManager::Scheduleable
//{
//public:
//	ScheduleableCommand(std::function<bool()> backingFunction, unsigned char requirementFlags)
//		:FunctionManager::Scheduleable(backingFunction, requirementFlags) {}
//};

class ScheduledCommand : public FunctionManager::Scheduleable
{
private:

public:
	ScheduledCommand(const std::function<bool()>& backingFunction, unsigned char requirementFlags)
		: FunctionManager::Scheduleable(backingFunction, requirementFlags) {}

	//ScheduledCommand(std::function<bool()> backingFunctionCopy, unsigned char requirementFlags, bool passingByCopy)
	//	: FunctionManager::Scheduleable(backingFunctionCopy, requirementFlags, passingByCopy) {}
};

//Link about Parameter Packs: https://en.cppreference.com/w/cpp/language/parameter_pack
template <typename... Ts>
class CommandBuilder
{
private:
	unsigned char requirementFlags;
	std::function<bool(Ts...)> backingFunction;

public:
	CommandBuilder(const std::function<bool(Ts...)>& backingFunction, unsigned char requirementFlags)
		:backingFunction{ backingFunction }, requirementFlags{ requirementFlags } {}

	CommandBuilder(const std::function<bool(Ts...)>& backingFunction, std::vector<Systems> requiredSystems)
		: CommandBuilder(backingFunction, GetRequirementFlag(requiredSystems)) {}

	CommandBuilder(const std::function<bool(Ts...)>& backingFunction, Systems system)
		: CommandBuilder(backingFunction, (unsigned char)system) {}

	//Command(std::function<bool(Ts...)> backingFunction, unsigned char requirementFlags, bool passByValue) //this shouldn't be necessary(remove it at some point)
	//	:Command(backingFunction, requirementFlags) 
	//{
	//	std::cout << "Command Made by Value" << std::endl;
	//}

#pragma region CommentedFunctions
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
#pragma endregion

	std::shared_ptr<ScheduledCommand> CreateCommand(const Ts&... params) //maybe this?
	{
		return std::make_shared<ScheduledCommand>([&]() {return backingFunction(params...); }, requirementFlags);
	}

	//std::shared_ptr<ScheduledCommand> ScheduleWith(bool useNothing) //function for debugging
	//{
	//	return std::make_shared<ScheduledCommand>([&]() {std::cout << "Yay this ran\n";  return true; }, requirementFlags, true);
	//}

	void SetInitializationBehavior()
	{

	}

	void SetEndBehavior()
	{

	}
};