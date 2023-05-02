#include "SchedulerBase.h"


//Scheduler::ScheduledFunction::ScheduledFunction(std::function<bool()>& backingFunction, unsigned char requirementFlags)
//	:backingFunction{ backingFunction }, RequirementFlags{ requirementFlags } {}

//bool Scheduler::ScheduledFunction::Run()
//{
//	return backingFunction();
//}


unsigned char SchedulerBase::GetRequirementFlags(std::vector<FunctionManager::Scheduleable*> scheduleables)
{
	unsigned char requirementFlag = 0;
	for (FunctionManager::Scheduleable* scheduleable : scheduleables)
	{
		requirementFlag |= scheduleable->GetRequirementFlags();
	}
	return requirementFlag;
}

SchedulerBase::SchedulerBase(unsigned char systemFlags, SchedulerTypes type) //change this to be the main constructor that the other constructors call
	:Scheduleable(std::function<bool()>([&]() {return Run(); }), systemFlags), functionManager{ FunctionManager() }, schedulerID{NextAvailableSchedulerID}
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

	NextAvailableSchedulerID++;
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
	int debug = schedulerID;
	unsigned char currentAvailableSystem = (unsigned char)0x1;
	std::vector<int> IDsToDelete = std::vector<int>();
	for (int i = 0; i < SystemsCount; i ++) //maybe use the requirementFlag instead of SystemsCount at some point
	{
		unsigned char currentMask = 1 << i;
		if ((requirementFlags & currentMask) >> i == 1)
		{
			Systems currentSystem = (Systems)(unsigned char)pow(2, i);
			std::list<int>& currentSystemSchedule = schedule[currentSystem];
			if (currentSystemSchedule.size() != 0 && functionManager.RunIfReady(currentSystemSchedule.front(), currentAvailableSystem))
			{
				IDsToDelete.push_back(currentSystemSchedule.front());
				functionManager.Remove(currentSystemSchedule.front());
			}
		}
		currentAvailableSystem <<= 1;
	}

	unsigned char finishedSystems = 0;
	for (int i = 0; i < SystemsCount; i ++)
	{
		unsigned char currentMask = 1 << i;
		if ((requirementFlags & currentMask) >> i == 1)
		{
			Systems currentSystem = (Systems)(unsigned char)pow(2, i);
			std::list<int>& currentSystemSchedule = schedule[currentSystem];
			if (currentSystemSchedule.size() == 0)
			{
				finishedSystems |= (unsigned char)currentSystem;
			}
			else
			{
				functionManager.ResetAvailability(currentSystemSchedule.front());
				for (int ID : IDsToDelete)
				{
					if (currentSystemSchedule.front() == ID)
					{
						currentSystemSchedule.pop_front();
						if (currentSystemSchedule.size() == 0)
						{
							finishedSystems |= (unsigned char)currentSystem;
						}
						break;
					}
				}
			}
		}
	}
	return finishedSystems == requirementFlags;
}