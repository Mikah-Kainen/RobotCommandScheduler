#include "FunctionManager.h"

#pragma region IScheduleable
FunctionManager::Scheduleable::Scheduleable(std::function<bool()> backingFunction, char requirementFlags)
	:backingFunction{ backingFunction }, requirementFlags{ requirementFlags }, availableSystems{(char)0}, IsDead{ false } {}

FunctionManager::Scheduleable::Scheduleable(std::function<bool()> backingFunction)
	:Scheduleable(backingFunction, (char)255) {}

FunctionManager::Scheduleable::Scheduleable() {}

FunctionManager::Scheduleable::~Scheduleable() 
{
	IsDead = true;
};

void FunctionManager::Scheduleable::AddRequirement(char newRequirementFlags)
{
	requirementFlags |= newRequirementFlags;
}

bool FunctionManager::Scheduleable::RunIfReady(char availableSystem) 
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
	:database{ std::unordered_map<int, Scheduleable>() }, nextAvailableID{ 0 } {}

int FunctionManager::AddToDatabase(Scheduleable scheduledItem)
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