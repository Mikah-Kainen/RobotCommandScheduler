#include "../include/schedulable.h"

Schedulable::SchedulableStates Schedulable::GetNextState(SchedulableStates state)
{
	return (SchedulableStates)((int)state + 1);
}

bool Schedulable::Run()
{
	return backingFunction();
}

void Schedulable::SetInitializationSchedulableDirect(std::shared_ptr<Schedulable> schedulable)
{
	initializationSchedulable = schedulable;
	startingState = SchedulableStates::Initialize;
}

void Schedulable::SetCleanupSchedulableDirect(std::shared_ptr<Schedulable> schedulable)
{
	cleanupSchedulable = schedulable;
	endingState = GetNextState(SchedulableStates::Cleanup);
}

Schedulable::Schedulable(unsigned char requirementFlags)
	:Schedulable(NoFunctionProvided, requirementFlags) {}

Schedulable::Schedulable(std::function<bool()> backingFunction, unsigned char requirementFlags)
	:backingFunction{ backingFunction }, initializationSchedulable{ nullptr }, cleanupSchedulable{nullptr},
	startingState {SchedulableStates::Run}, endingState{ GetNextState(SchedulableStates::Run) }, currentState{SchedulableStates::Run}, 
	requirementFlags {requirementFlags}, availableSystems{ (unsigned char)0 }, IsDead{ false } {}

Schedulable::Schedulable(std::function<bool()> backingFunction, Systems requiredSystem)
	:Schedulable(backingFunction, (unsigned char)requiredSystem) {}

//Schedulable::Schedulable(std::function<bool()> backingFunctionCopy, unsigned char requirementFlags, bool passingByCopy)
//	:Schedulable{ backingFunction, requirementFlags } 
//{
//	std::cout << "Schedulable Backing Function Made via Copy!" << std::endl;
//}
//
//Schedulable::Schedulable(std::function<bool()> backingFunctionCopy, Systems requiredSystem, bool passingByCopy)
//	:Schedulable(backingFunctionCopy, (unsigned char)requiredSystem) 
//{
//	std::cout << "Schedulable Backing Function Made via Copy!" << std::endl;
//}

//maybe this required for unordered_map?
//Schedulable::Schedulable() {}

Schedulable::Schedulable(const Schedulable& copySchedulable)
	:availableSystems{ copySchedulable.availableSystems }, backingFunction{ copySchedulable.backingFunction }, initializationSchedulable{ copySchedulable.initializationSchedulable }, 
	cleanupSchedulable{ copySchedulable.cleanupSchedulable }, startingState{ copySchedulable.startingState }, endingState{ copySchedulable.endingState },
	currentState{ copySchedulable.currentState }, IsDead{ copySchedulable.IsDead }, requirementFlags{copySchedulable.requirementFlags}
{
	//std::cout << "Schedulable Copied!!" << std::endl;
}

Schedulable::~Schedulable() 
{
	if (IsDead)
	{
		//throw new std::exception("THIS IS ALREADY DEAD!\n");
		std::cout << "THIS IS ALREADY DEAD!\n" << std::endl;
		while (true);
	}
	IsDead = true;
	//std::cout << "Schedulable Destructed\n";
};

unsigned char Schedulable::GetRequirementFlags()
{
	return requirementFlags;
}

void Schedulable::AddRequirement(unsigned char newRequirementFlags)
{
	requirementFlags |= newRequirementFlags;
}

void Schedulable::AddAvailability(unsigned char availableSystem)
{
	availableSystems |= availableSystem;
}

bool Schedulable::IsReady()
{
	return (availableSystems & requirementFlags) == requirementFlags;
}

bool Schedulable::RunFSM()
{
	switch (currentState)
	{
	case SchedulableStates::Initialize:
		if (initializationSchedulable->RunFSM())
		{
			currentState = GetNextState(currentState);
			return currentState == endingState;
		}
		break;

	case SchedulableStates::Run:
		if (Run())
		{
			currentState = GetNextState(currentState);
			return currentState == endingState;
		}
		break;

	case SchedulableStates::Cleanup:
		if (cleanupSchedulable->RunFSM())
		{
			currentState = GetNextState(currentState);
			return currentState == endingState;
		}
		break;

	case SchedulableStates::Return:
		std::cout << "IMPOSSIBLE STATE" << std::endl;
		while (true);
		break;

	default:
		std::cout << "IMPOSSIBLE STATE" << std::endl;
		while (true);
		break;
	}
	return false;
}

void Schedulable::Initialize()
{
	currentState = startingState;
	if (startingState == SchedulableStates::Initialize)
	{
		initializationSchedulable->Initialize();
	}
	if (endingState == SchedulableStates::Return)
	{
		cleanupSchedulable->Initialize();
	}
}

//void Schedulable::InitializeFSM()
//{
//	Initialize(); //figutre out this
//	if (startingState == SchedulableStates::Initialize)
//	{
//		initializationSchedulable->InitializeFSM();
//	}
//	if (endingState == SchedulableStates::Return)
//	{
//		cleanupSchedulable->InitializeFSM();
//	}
//}

//bool Schedulable::RunIfReady(unsigned char availableSystem) 
////An ISchedulable that doesn't require any systems will never run(unless availableSystem is 0) because RunIfReady always adds an available system before checking the requirements 
//{
//	if (IsDead)
//	{
//		std::cout << "THIS ALREADY DIED!! ALERT, THIS IS DEAD ALREADY!";
//	}
//  availableSystems |= availableSystem;
//	if ((availableSystems & requirementFlags) == requirementFlags)
//	{
//		switch (currentState)
//		{
//		case SchedulableStates::Initialize:
//			if (initializationSchedulable->Run())
//			{
//				currentState = GetNextState(currentState);
//				return currentState == endingState;
//			}
//			break;
//
//		case SchedulableStates::Run:
//			if (Run())
//			{
//				currentState = GetNextState(currentState);
//				return currentState == endingState;
//			}
//			break;
//
//		case SchedulableStates::Cleanup:
//			if (cleanupSchedulable->Run())
//			{
//				currentState = GetNextState(currentState);
//				return currentState == endingState;
//			}
//			break;
//
//		case SchedulableStates::Return:
//			std::cout << "IMPOSSIBLE STATE" << std::endl;
//			while (true);
//			break;
//
//		default:
//			std::cout << "IMPOSSIBLE STATE" << std::endl;
//			while (true);
//			break;
//		}
//	}
//	else
//	{
//		return false;
//	}
//}

void Schedulable::ResetAvailability()
{
	availableSystems = 0;
}

bool Schedulable::SetInitializationSchedulable(std::shared_ptr<Schedulable> schedulable)
{
	if (IsSubset(schedulable->requirementFlags, requirementFlags))
	{
		SetInitializationSchedulableDirect(schedulable);
		return true;
	}
	return false;
}

bool Schedulable::SetCleanupSchedulable(std::shared_ptr<Schedulable> schedulable)
{
	if (IsSubset(schedulable->requirementFlags, requirementFlags))
	{
		SetCleanupSchedulableDirect(schedulable);
		return true;
	}
	return false;
}