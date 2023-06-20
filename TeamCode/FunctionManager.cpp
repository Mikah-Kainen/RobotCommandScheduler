//#include <iostream>
////#include "../../inc/SchedulerInc/FunctionManager.h"
//#include "FunctionManager.h"
//
//#pragma region Scheduleable
//
//bool Scheduleable::Run()
//{
//	return backingFunction();
//}
//
//Scheduleable::Scheduleable(unsigned char requirementFlags)
//	:Scheduleable(NoFunctionProvided, requirementFlags) {}
//
//Scheduleable::Scheduleable(std::function<bool()> backingFunction, unsigned char requirementFlags)
//	:backingFunction{ backingFunction }, requirementFlags{ requirementFlags }, availableSystems{(unsigned char)0}, IsDead{ false } {}
//
//Scheduleable::Scheduleable(std::function<bool()> backingFunction, Systems requiredSystem)
//	:Scheduleable(backingFunction, (unsigned char)requiredSystem) {}
//
////Scheduleable::Scheduleable(std::function<bool()> backingFunctionCopy, unsigned char requirementFlags, bool passingByCopy)
////	:Scheduleable{ backingFunction, requirementFlags } 
////{
////	std::cout << "Scheduleable Backing Function Made via Copy!" << std::endl;
////}
////
////Scheduleable::Scheduleable(std::function<bool()> backingFunctionCopy, Systems requiredSystem, bool passingByCopy)
////	:Scheduleable(backingFunctionCopy, (unsigned char)requiredSystem) 
////{
////	std::cout << "Scheduleable Backing Function Made via Copy!" << std::endl;
////}
//
////maybe this required for unordered_map?
////Scheduleable::Scheduleable() {}
//
//Scheduleable::Scheduleable(const Scheduleable& copyScheduleable)
//	:availableSystems{ copyScheduleable.availableSystems }, backingFunction{ copyScheduleable.backingFunction }, IsDead{ copyScheduleable.IsDead }, requirementFlags{copyScheduleable.requirementFlags}
//{
//	//std::cout << "Scheduleable Copied!!" << std::endl;
//}
//
//Scheduleable::~Scheduleable() 
//{
//	if (IsDead)
//	{
//		throw new std::exception("THIS IS ALREADY DEAD!\n");
//	}
//	IsDead = true;
//	std::cout << "Scheduleable Destructed\n";
//};
//
//unsigned char Scheduleable::GetRequirementFlags()
//{
//	return requirementFlags;
//}
//
//void Scheduleable::AddRequirement(unsigned char newRequirementFlags)
//{
//	requirementFlags |= newRequirementFlags;
//}
//
//bool Scheduleable::RunIfReady(unsigned char availableSystem) 
////An IScheduleable that doesn't require any systems will never run(unless availableSystem is 0) because RunIfReady always adds an available system before checking the requirements 
//{
//	if (IsDead)
//	{
//		std::cout << "THIS ALREADY DIED!! ALERT, THIS IS DEAD ALREADY!";
//	}
//	availableSystems |= availableSystem;
//	if ((availableSystems & requirementFlags) == requirementFlags)
//	{
//		return Run();
//	}
//	else
//	{
//		return false;
//	}
//}
//
//void Scheduleable::ResetAvailability()
//{
//	availableSystems = 0;
//}
//#pragma endregion
//
//
//#pragma region FunctionManager
//
//FunctionManager::FunctionManager()
//	:database{ std::unordered_map<unsigned int, std::shared_ptr<Scheduleable>>() }, endBehaviors{std::unordered_map<unsigned int, std::vector<std::function<void()>>>()}, nextAvailableID{0} {}
//
//FunctionManager::FunctionManager(const FunctionManager& copyFunctionManager)
//	: nextAvailableID{ copyFunctionManager.nextAvailableID }, database{ std::unordered_map<unsigned int, std::shared_ptr<Scheduleable>>() }, endBehaviors{ std::unordered_map<unsigned int, std::vector<std::function<void()>>>() }
//{
//	for (std::pair<unsigned int, std::shared_ptr<Scheduleable>> kvp : copyFunctionManager.database)
//	{
//		database.emplace(kvp.first, kvp.second);
//	}
//
//	for (std::pair<unsigned int, std::vector<std::function<void()>>> kvp : copyFunctionManager.endBehaviors)
//	{
//		std::vector<std::function<void()>> targetBehaviors = std::vector<std::function<void()>>();
//		for (std::function<void()> behavior : kvp.second)
//		{
//			targetBehaviors.push_back(behavior);
//		}
//		endBehaviors.emplace(kvp.first, targetBehaviors);
//	}
//}
//
//FunctionManager::~FunctionManager()
//{
//	//I should do something in the destructors
//}
//
//unsigned int FunctionManager::AddToDatabase(std::shared_ptr<Scheduleable> scheduledItem)
//{
//	unsigned int ID = nextAvailableID++;
//	database.emplace(ID, scheduledItem);
//	endBehaviors.emplace(ID, std::vector<std::function<void()>>());
//	return ID;
//}
//
//bool FunctionManager::RunIfReady(unsigned int scheduledID, unsigned char availableSystem)
//{
//	bool result = database[scheduledID]->RunIfReady(availableSystem);
//	if (result)
//	{
//		for (std::function<void()>& behavior : endBehaviors[scheduledID])
//		{
//			behavior();
//		}
//		//do end of function logic
//		//throw std::exception("not implemented"); //UNCOMMENT THIS!!
//		//Remove(scheduledID); //Don't delete here because I clean up in GroupBase(I could change Cleanup here if I want so maybe think about that)
//	}
//	return result;
//}
//
//void FunctionManager::Remove(unsigned int ID)
//{
//	database.erase(ID);
//	endBehaviors.erase(ID);
//	//remove ID from database
//}
//
//void FunctionManager::ResetAvailability(unsigned int ID)
//{
//	database[ID]->ResetAvailability();
//}
//
//void FunctionManager::SubscribeToEnd(unsigned int targetID, std::function<void()> endBehavior)
//{
//	endBehaviors[targetID].push_back(endBehavior);
//}
//
//#pragma endregion