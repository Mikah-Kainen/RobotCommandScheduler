#include <iostream>
//#include "../../inc/SchedulerInc/FunctionManager.h"
#include "FunctionManager.h"

#pragma region IScheduleable
FunctionManager::Scheduleable::Scheduleable(const std::function<bool()>& backingFunction, unsigned char requirementFlags)
	:backingFunction{ backingFunction }, requirementFlags{ requirementFlags }, availableSystems{(unsigned char)0}, IsDead{ false } {}

FunctionManager::Scheduleable::Scheduleable(const std::function<bool()>& backingFunction, Systems requiredSystem)
	:Scheduleable(backingFunction, (unsigned char)requiredSystem) {}

//maybe this required for unordered_map?
//FunctionManager::Scheduleable::Scheduleable() {}

FunctionManager::Scheduleable::~Scheduleable() 
{
	if (IsDead)
	{
		throw new std::exception("THIS IS ALREADY DEAD!\n");
	}
	IsDead = true;
	std::cout << "FunctionDied\n";
};

unsigned char FunctionManager::Scheduleable::GetRequirementFlags()
{
	return requirementFlags;
}

void FunctionManager::Scheduleable::AddRequirement(unsigned char newRequirementFlags)
{
	requirementFlags |= newRequirementFlags;
}

bool FunctionManager::Scheduleable::RunIfReady(unsigned char availableSystem) 
//An IScheduleable that doesn't require any systems will never run(unless availableSystem is 0) because RunIfReady always adds an available system before checking the requirements 
{
	if (IsDead)
	{
		std::cout << "THIS ALREADY DIED!! ALERT, THIS IS DEAD ALREADY!";
	}
	availableSystems |= availableSystem;
	if ((availableSystems & requirementFlags) == requirementFlags)
	{
		return backingFunction();
	}
	else
	{
		return false;
	}
}

void FunctionManager::Scheduleable::ResetAvailability()
{
	availableSystems = 0;
}
#pragma endregion


#pragma region FunctionManager

FunctionManager::FunctionManager()
	:database{ std::unordered_map<int, std::shared_ptr<Scheduleable>>() }, nextAvailableID{ 0 } {}

int FunctionManager::AddToDatabase(std::shared_ptr<Scheduleable> scheduledItem)
{
	int ID = nextAvailableID++;
	database.emplace(ID, scheduledItem);
	return ID;
}

bool FunctionManager::RunIfReady(int scheduledID, unsigned char availableSystem)
{
	bool result = database[scheduledID]->RunIfReady(availableSystem);
	if (result)
	{
		//do end of function logic
		//throw std::exception("not implemented"); //UNCOMMENT THIS!!
		Remove(scheduledID);
	}
	return result;
}

void FunctionManager::Remove(int ID)
{
	database.erase(ID);
	//remove ID from database
}

void FunctionManager::ResetAvailability(int ID)
{
	database[ID]->ResetAvailability();
}

#pragma endregion