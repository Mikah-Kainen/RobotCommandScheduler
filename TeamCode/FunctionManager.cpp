#include "FunctionManager.h"

#pragma region IScheduleable
FunctionManager::IScheduleable::IScheduleable(std::function<bool()> backingFunction, char requirementFlags)
	:backingFunction{ backingFunction }, requirementFlags{ requirementFlags }, availableSystems{(char)0} {}

FunctionManager::IScheduleable::IScheduleable(std::function<bool()> backingFunction)
	:IScheduleable(backingFunction, (char)0) {}

void FunctionManager::IScheduleable::AddRequirement(char newRequirementFlags)
{
	requirementFlags |= newRequirementFlags;
}

bool FunctionManager::IScheduleable::RunIfReady(char availableSystem) 
//An IScheduleable that doesn't require any systems will never run because RunIfReady always adds an available system before checking the requirements
{
	availableSystems |= availableSystem;
	if (availableSystems == requirementFlags)
	{
		return backingFunction();
	}
	else
	{
		return false;
	}
}
#pragma endregion


#pragma region FunctionManager

FunctionManager::FunctionManager()
	:database{ std::unordered_map<int, std::unique_ptr<IScheduleable>>() }, nextAvailableID{0} {}

int FunctionManager::AddToDatabase(IScheduleable& scheduledItem)
{
	int ID = nextAvailableID++;
	auto newPair = std::pair<const int, std::unique_ptr<IScheduleable>>(ID, std::make_unique<IScheduleable>(scheduledItem));
	database.insert(newPair);
	return ID;
}

bool FunctionManager::RunIfReady(int scheduledID, char availableSystem)
{
	bool result = database[scheduledID]->RunIfReady(availableSystem);
	if (result)
	{
		//do end of function logic
		//remove from database
		throw std::exception("not implemented");
	}
	return result;
}

#pragma endregion