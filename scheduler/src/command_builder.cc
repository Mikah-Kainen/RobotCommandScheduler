#pragma once
#include <functional>
#include <exception>
#include <iostream>
#include <tuple>
#include <utility>
#include <memory>

#include "../include/schedulable.h"
#include "../include/static.h"
#include "../include/schedulable_groups/condition_group.h"
#include "polyfills.cc"

//class SchedulableCommand : Schedulable
//{
//public:
//	SchedulableCommand(std::function<bool()> backingFunction, unsigned char requirementFlags)
//		:Schedulable(backingFunction, requirementFlags) {}
//};

#pragma region ClassDeclarations
//https://stackoverflow.com/questions/15537817/c-how-to-store-a-parameter-pack-as-a-variable //Look at this link for some interesting ideas
//https://en.cppreference.com/w/cpp/utility/apply
template <typename... Ts>
class ScheduledCommand : public Schedulable
{
	template <typename... Ts2>
	friend class CommandBuilder;

private:
	std::function<bool(Ts...)> backingFunction;
	std::tuple<Ts...> params;

	bool Run() override;

public:
	ScheduledCommand(std::function<bool(Ts...)> backingFunction, Ts... params, unsigned char requirementFlags);

	//ScheduledCommand(std::function<bool()> backingFunctionCopy, unsigned char requirementFlags, bool passingByCopy);
};

//Link about Parameter Packs: https://en.cppreference.com/w/cpp/language/parameter_pack
//Make builder a base class
template <typename... Ts>
class CommandBuilder
{
private:
	unsigned char requirementFlags;
	std::function<bool(Ts...)> backingFunction;
	std::shared_ptr<Schedulable> initializationSchedulable;
	std::shared_ptr<Schedulable> cleanupSchedulable;

public:
	CommandBuilder(std::function<bool(Ts...)> backingFunction, unsigned char requirementFlags);

	CommandBuilder(std::function<bool(Ts...)> backingFunction, std::vector<Systems> requiredSystems);

	CommandBuilder(std::function<bool(Ts...)> backingFunction, Systems system);

	//Command(std::function<bool(Ts...)> backingFunction, unsigned char requirementFlags, bool passByValue); //this shouldn't be necessary(remove it at some point)

#pragma region CommentedFunctions
	//Use for testing purposes if needed
	//bool Activate(Ts... params);

	//This function isn't bad but at the same time I don't really see a good use case and it's not so good if commands without requirements exist so I'll force the constructor to take the requirements
	//void AddRequirement(Systems requirement);
#pragma endregion

	std::shared_ptr<ScheduledCommand<Ts...>> CreateCommand(Ts... params);

	//std::shared_ptr<ScheduledCommand> ScheduleWith(bool useNothing); //function for debugging

	bool SetInitialization(std::shared_ptr<Schedulable> schedulable);

	bool SetCleanup(std::shared_ptr<Schedulable> schedulable);
};
#pragma endregion



#pragma region ClassDefinitions
template <typename... Ts>
bool ScheduledCommand<Ts...>::Run()
{
	//return std::apply(backingFunction, params);
	return PolyFills::GetInstance().Apply(backingFunction, params);
}

template <typename... Ts>
ScheduledCommand<Ts...>::ScheduledCommand(std::function<bool(Ts...)> backingFunction, Ts... params, unsigned char requirementFlags)
	: Schedulable(requirementFlags), backingFunction{ backingFunction }, params{std::tuple<Ts...>(params...)} {}

//template <typename... Ts>
//ScheduledCommand<Ts...>::ScheduledCommand(std::function<bool()> backingFunctionCopy, unsigned char requirementFlags, bool passingByCopy)
//	: Schedulable(backingFunctionCopy, requirementFlags, passingByCopy) {}

//Link about Parameter Packs: https://en.cppreference.com/w/cpp/language/parameter_pack
//Make builder a base class
template <typename... Ts>
CommandBuilder<Ts...>::CommandBuilder(std::function<bool(Ts...)> backingFunction, unsigned char requirementFlags)
	:backingFunction{ backingFunction }, requirementFlags{ requirementFlags } {}

template <typename... Ts>
CommandBuilder<Ts...>::CommandBuilder(std::function<bool(Ts...)> backingFunction, std::vector<Systems> requiredSystems)
	: CommandBuilder(backingFunction, GetRequirementFlag(requiredSystems)) {}

template <typename... Ts>
CommandBuilder<Ts...>::CommandBuilder(std::function<bool(Ts...)> backingFunction, Systems system)
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

template <typename... Ts>
std::shared_ptr<ScheduledCommand<Ts...>> CommandBuilder<Ts...>::CreateCommand(Ts... params)
{
	std::shared_ptr<ScheduledCommand<Ts...>> returnSchedulable = std::make_shared<ScheduledCommand<Ts...>>(backingFunction, params..., requirementFlags);
	if (initializationSchedulable)
	{
		returnSchedulable->Schedulable::SetInitializationSchedulableDirect(initializationSchedulable);
	}
	if (cleanupSchedulable)
	{
		returnSchedulable->Schedulable::SetCleanupSchedulableDirect(cleanupSchedulable);
	}
	return returnSchedulable;
}

template <typename... Ts>
bool CommandBuilder<Ts...>::SetInitialization(std::shared_ptr<Schedulable> schedulable) //Fails if schedulable requirements are not a subset of requirementFlags
{
	if (!IsSubset(schedulable->requirementFlags, requirementFlags))
	{
		return false;
	}

	initializationSchedulable = schedulable;
	return true;
}

template <typename... Ts>
bool CommandBuilder<Ts...>::SetCleanup(std::shared_ptr<Schedulable> schedulable) //Fails if schedulable requirements are not a subset of requirementFlags
{
	if (!IsSubset(schedulable->requirementFlags, requirementFlags))
	{
		return false;
	}

	cleanupSchedulable = schedulable;
	return true;
}

#pragma endregion