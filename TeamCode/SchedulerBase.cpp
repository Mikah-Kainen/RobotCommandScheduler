#include "SchedulerBase.h"


//Scheduler::ScheduledFunction::ScheduledFunction(std::function<bool()>& backingFunction, unsigned char requirementFlags)
//	:backingFunction{ backingFunction }, RequirementFlags{ requirementFlags } {}

//bool Scheduler::ScheduledFunction::Run()
//{
//	return backingFunction();
//}


unsigned char SchedulerBase::GetRequirementFlags(std::vector<FunctionManager::Scheduleable*> scheduleables)
{
	unsigned char requirementFlag;
	for (FunctionManager::Scheduleable* scheduleable : scheduleables)
	{
		requirementFlag |= scheduleable->GetRequirementFlags();
	}
	return requirementFlag;
}

SchedulerBase::SchedulerBase(unsigned char systemFlags, SchedulerTypes type) //change this to be the main constructor that the other constructors call
	:Scheduleable(std::function<bool()>([&]() {return Run(); }), systemFlags), functionManager{ FunctionManager() }
{
	schedulerType = type;

	schedule = std::unordered_map<Systems, std::list<int>>();
	for (int i = 0; i < SystemsCount; i++)
	{
		unsigned char currentMask = 1 << i;
		if ((systemFlags & currentMask) >> i == 1)
		{
			Systems currentSystem = (Systems)(unsigned char)pow(2, i);
			schedule.emplace(currentSystem, std::list<int>());
		}
	}
}

SchedulerBase::SchedulerBase(std::vector<unsigned char> systemFlags, SchedulerTypes type)
	:SchedulerBase(GetRequirementFlag(systemFlags), type) {}

SchedulerBase::SchedulerBase(std::vector<Systems> schedulerSystems, SchedulerTypes type)
	:SchedulerBase(GetRequirementFlag(schedulerSystems), type) {}


//make it so commands can be scheduled with their parameters passed in(and the FunctionManager properly owns everything)

//add additional scheduling functions to handle scheduling Schedulers
//also think about how conditional functions could work(maybe a placeholder function that is replaced by a Scheduler that handles whichever branch it is currently on)
//also add functionality to remove systems from the scheduler when all the system's commands have been completed, so that the parent scheduler can start scheduling things with that system(but don't remove systems from the original scheduler!)
//make different types of schedulers or have information in the schedulers which decides whether a system should be removed or its default functions should be run when the scheduler runs out of scheduled functions for that system
void SchedulerBase::Schedule(Scheduleable* scheduleable)
{
#pragma region workingTheory
	/*
	* char needs to be unsigned
	* use masks instead of shifting
	void SchedulerTest(unsigned char requirementFlags)
{
	for (int i = 0; i < 8; i++)
	{
		unsigned char currentMask = 1 << i;
		if ((requirementFlags & currentMask) >> i == 1)
		{
			std::cout << (pow(2, i)) << std::endl;
			//schedule[(Systems)pow(2, i)].push_back(newID);
		}
	}
	//Definitely put stuff here
}*/
#pragma endregion
	unsigned char requirementFlags = scheduleable->GetRequirementFlags();
	int newID = functionManager.AddToDatabase(scheduleable);
	for (int i = 0; i < SystemsCount; i++)
	{
		unsigned char currentMask = 1 << i;
		if ((requirementFlags & currentMask) >> i == 1)
		{
			Systems currentSystem = (Systems)(unsigned char)pow(2, i);
			schedule[currentSystem].push_back(newID);
		}
	}
	//Definitely put stuff here
}

void SchedulerBase::Schedule(std::function<bool()> function, unsigned char requirementFlags)
{
	Schedule(new ScheduledCommand(function, requirementFlags));
}

void SchedulerBase::Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems)
{
	Schedule(function, GetRequirementFlag(requiredSystems));
}

void SchedulerBase::Update()
{
	//This is probably unnecessary
}

bool SchedulerBase::Run()
{
	unsigned char availableSystem = (unsigned char)0x1;
	for (int i = 0; i < SystemsCount; i ++)
	{
		unsigned char currentMask = 1 << i;
		if ((requirementFlags & currentMask) >> i == 1)
		{
			Systems currentSystem = (Systems)(unsigned char)pow(2, i);
			std::list<int>& currentSystemSchedule = schedule[currentSystem];
			if (currentSystemSchedule.size() == 0) //Optimally, this check should be nested right after functions are removed from the functionManager, but I'll keep it here for now because the BaseScheduler doesn't run default functions yet, meaning schedules of size 0 exist in the BaseScheduler
			{
				schedule.erase(currentSystem);
				requirementFlags = requirementFlags & (255 - currentMask); //Test this
				std::cout << "DeletedSystem: " << (int)currentSystem << " Size: " << schedule.size() << std::endl;
				//std::cout << "No functions are schedule for System-" << i << "\n";
			}
			else if (functionManager.RunIfReady(currentSystemSchedule.front(), availableSystem))
			{
				functionManager.Remove(currentSystemSchedule.front());
				currentSystemSchedule.pop_front();
			}
			availableSystem <<= 1;
		}
		//std::unique_ptr<ScheduledFunction>& currentFunction = functions.Get(currentSystemSchedule.front());
		//if (currentFunction->RequirementFlags & currentlyRunningSystems == 0)
		//{
		//	currentFunction->AvailableSystems |= (1 << i);
		//	if (currentFunction->AvailableSystems == currentFunction->RequirementFlags)
		//	{
		//		currentlyRunningSystems |= currentFunction->RequirementFlags;
		//		if (currentFunction->Run())
		//		{
		//			currentSystemSchedule.pop_front();
		//		}
		//		//think about if I should ever reset the AvailableSystems of a function(maybe in Update?)
		//		//(i.e. does a system which was once available for ever become unavailable for that function before that function finishes?)
		//	}
		//}
	}
	return schedule.size() == 0;
}