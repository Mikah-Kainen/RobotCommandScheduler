#include "Scheduleable.h"

bool Scheduleable::Run()
{
	return backingFunction();
}

Scheduleable::Scheduleable(unsigned char requirementFlags)
	:Scheduleable(NoFunctionProvided, requirementFlags) {}

Scheduleable::Scheduleable(std::function<bool()> backingFunction, unsigned char requirementFlags)
	:backingFunction{ backingFunction }, requirementFlags{ requirementFlags }, availableSystems{(unsigned char)0}, IsDead{ false } {}

Scheduleable::Scheduleable(std::function<bool()> backingFunction, Systems requiredSystem)
	:Scheduleable(backingFunction, (unsigned char)requiredSystem) {}

//Scheduleable::Scheduleable(std::function<bool()> backingFunctionCopy, unsigned char requirementFlags, bool passingByCopy)
//	:Scheduleable{ backingFunction, requirementFlags } 
//{
//	std::cout << "Scheduleable Backing Function Made via Copy!" << std::endl;
//}
//
//Scheduleable::Scheduleable(std::function<bool()> backingFunctionCopy, Systems requiredSystem, bool passingByCopy)
//	:Scheduleable(backingFunctionCopy, (unsigned char)requiredSystem) 
//{
//	std::cout << "Scheduleable Backing Function Made via Copy!" << std::endl;
//}

//maybe this required for unordered_map?
//Scheduleable::Scheduleable() {}

Scheduleable::Scheduleable(const Scheduleable& copyScheduleable)
	:availableSystems{ copyScheduleable.availableSystems }, backingFunction{ copyScheduleable.backingFunction }, IsDead{ copyScheduleable.IsDead }, requirementFlags{copyScheduleable.requirementFlags}
{
	//std::cout << "Scheduleable Copied!!" << std::endl;
}

Scheduleable::~Scheduleable() 
{
	if (IsDead)
	{
		throw new std::exception("THIS IS ALREADY DEAD!\n");
	}
	IsDead = true;
	std::cout << "Scheduleable Destructed\n";
};

unsigned char Scheduleable::GetRequirementFlags()
{
	return requirementFlags;
}

void Scheduleable::AddRequirement(unsigned char newRequirementFlags)
{
	requirementFlags |= newRequirementFlags;
}

bool Scheduleable::RunIfReady(unsigned char availableSystem) 
//An IScheduleable that doesn't require any systems will never run(unless availableSystem is 0) because RunIfReady always adds an available system before checking the requirements 
{
	if (IsDead)
	{
		std::cout << "THIS ALREADY DIED!! ALERT, THIS IS DEAD ALREADY!";
	}
	availableSystems |= availableSystem;
	if ((availableSystems & requirementFlags) == requirementFlags)
	{
		return Run();
	}
	else
	{
		return false;
	}
}

void Scheduleable::ResetAvailability()
{
	availableSystems = 0;
}