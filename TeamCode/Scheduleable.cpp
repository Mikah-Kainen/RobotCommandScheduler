//#include "../../inc/SchedulerInc/Scheduleable.h"
#include "Scheduleable.h"


Scheduleable::ScheduleableStates Scheduleable::GetNextState(ScheduleableStates state)
{
	return (ScheduleableStates)((int)ScheduleableStates::Run + 1);
}

bool Scheduleable::Run()
{
	return backingFunction();
}

Scheduleable::Scheduleable(unsigned char requirementFlags)
	:Scheduleable(NoFunctionProvided, requirementFlags) {}

Scheduleable::Scheduleable(std::function<bool()> backingFunction, unsigned char requirementFlags)
	:backingFunction{ backingFunction }, startingState{ ScheduleableStates::Run }, endingState{ GetNextState(ScheduleableStates::Run)}, currentState{ScheduleableStates::Run},
	requirementFlags {requirementFlags}, availableSystems{ (unsigned char)0 }, IsDead{ false } {}

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
	:availableSystems{ copyScheduleable.availableSystems }, backingFunction{ copyScheduleable.backingFunction }, initializationScheduleable{ copyScheduleable.initializationScheduleable }, 
	cleanupScheduleable{copyScheduleable.cleanupScheduleable}, startingState {copyScheduleable.startingState}, endingState{ copyScheduleable.endingState }, 
	currentState{ copyScheduleable.currentState }, IsDead{ copyScheduleable.IsDead }, requirementFlags{copyScheduleable.requirementFlags}
{
	//std::cout << "Scheduleable Copied!!" << std::endl;
}

Scheduleable::~Scheduleable() 
{
	if (IsDead)
	{
		//throw new std::exception("THIS IS ALREADY DEAD!\n");
		std::cout << "THIS IS ALREADY DEAD!\n" << std::endl;
		while (true);
	}
	IsDead = true;
	//std::cout << "Scheduleable Destructed\n";
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
		switch (currentState)
		{
		case ScheduleableStates::Initialize:
			if (initializationScheduleable->Run())
			{
				currentState = GetNextState(currentState);
				return currentState == endingState;
			}
			break;

		case ScheduleableStates::Run:
			if (Run())
			{
				currentState = GetNextState(currentState);
				return currentState == endingState;
			}
			break;

		case ScheduleableStates::Cleanup:
			if (cleanupScheduleable->Run())
			{
				currentState = GetNextState(currentState);
				return currentState == endingState;
			}
			break;

		case ScheduleableStates::Return:
			std::cout << "IMPOSSIBLE STATE" << std::endl;
			while (true);
			break;

		default:
			std::cout << "IMPOSSIBLE STATE" << std::endl;
			while (true);
			break;
		}
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

bool Scheduleable::Initialize()
{
	currentState = startingState;
	return true;
}

bool Scheduleable::SetInitializationScheduleable(std::shared_ptr<Scheduleable> scheduleable)
{
	initializationScheduleable = scheduleable;
	startingState = ScheduleableStates::Initialize;
	return true;
}

bool Scheduleable::SetCleanupScheduleable(std::shared_ptr<Scheduleable> scheduleable)
{
	cleanupScheduleable = scheduleable;
	endingState = GetNextState(ScheduleableStates::Cleanup);
	return true;
}