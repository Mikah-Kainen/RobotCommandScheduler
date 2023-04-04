#include "FunctionManager.h"

#pragma region IScheduleable
FunctionManager::IScheduleable::IScheduleable(std::function<bool()> backingFunction, char requirementFlags)
	:backingFunction{ backingFunction }, requirementFlags{ requirementFlags }, availableSystems{(char)0}, IsDead{ false } {}

FunctionManager::IScheduleable::IScheduleable(std::function<bool()> backingFunction)
	:IScheduleable(backingFunction, (char)0) {}

FunctionManager::IScheduleable::IScheduleable() {}

FunctionManager::IScheduleable::~IScheduleable() 
{
	IsDead = true;
};

void FunctionManager::IScheduleable::AddRequirement(char newRequirementFlags)
{
	requirementFlags |= newRequirementFlags;
}

bool FunctionManager::IScheduleable::RunIfReady(char availableSystem) 
//An IScheduleable that doesn't require any systems will never run because RunIfReady always adds an available system before checking the requirements
{
	if (IsDead)
	{
		throw std::exception("This is not alive");
	}
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
	:database{ std::unordered_map<int, IScheduleable>() }, nextAvailableID{ 0 } {}

int FunctionManager::AddToDatabase(IScheduleable scheduledItem)
{
	int ID = nextAvailableID++;
	database.emplace(ID, scheduledItem);
	return ID;
}

bool FunctionManager::RunIfReady(int scheduledID, char availableSystem)
{
	bool result = database[scheduledID].RunIfReady(availableSystem);
	if (result)
	{
		//do end of function logic
		//throw std::exception("not implemented"); //UNCOMMENT THIS!!
	}
	return result;
}

void FunctionManager::Remove(int ID)
{
	//remove ID from database
}

#pragma endregion