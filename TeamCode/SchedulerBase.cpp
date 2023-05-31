//#include "../../inc/SchedulerInc/SchedulerBase.h"
#include "SchedulerBase.h"

//Scheduler::ScheduledFunction::ScheduledFunction(std::function<bool()>& backingFunction, unsigned char requirementFlags)
//	:backingFunction{ backingFunction }, RequirementFlags{ requirementFlags } {}

//bool Scheduler::ScheduledFunction::Run()
//{
//	return backingFunction();
//}


unsigned char SchedulerBase::GetRequirementFlags(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleables)
{
	unsigned char requirementFlag = 0;
	for (std::shared_ptr<FunctionManager::Scheduleable> scheduleable : scheduleables)
	{
		requirementFlag |= scheduleable->GetRequirementFlags();
	}
	return requirementFlag;
}

SchedulerBase::SchedulerBase(unsigned char systemFlags, SchedulerTypes type) //change this to be the main constructor that the other constructors call
	:Scheduleable(std::function<bool()>([&]() {return Run(); }/*std::function<bool()>(std::bind(&SchedulerBase::Run, this)*/), systemFlags), functionManager{ FunctionManager() }, schedulerID{ NextAvailableSchedulerID }
	//I don't use std::bing because it causes a copy anyway when passed to the next function despite being passed as a reference(I think)
{
	schedulerType = type;

	schedule = std::unordered_map<Systems, std::list<int>>();
	for (int i = 0; i < SystemsCount; i++)
	{
		unsigned char currentMask = 1 << i;
		if ((systemFlags & currentMask) >> i == 1)
		{
			Systems currentSystem = (Systems)currentMask;
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
void SchedulerBase::Schedule(std::shared_ptr<Scheduleable> scheduleable)
{
	unsigned char requirementFlags = scheduleable->GetRequirementFlags();
	int newID = functionManager.AddToDatabase(scheduleable);
	for (int i = 0; i < SystemsCount; i++)
	{
		unsigned char currentMask = 1 << i;
		if ((requirementFlags & currentMask) >> i == 1)
		{
			Systems currentSystem = (Systems)currentMask;
			schedule[currentSystem].push_back(newID);
		}
	}
	//Definitely put stuff here
}

void SchedulerBase::Schedule(const std::function<bool()>& function, unsigned char requirementFlags)
{
	Schedule(std::make_shared<Scheduleable>(function, requirementFlags));
}

void SchedulerBase::Schedule(const std::function<bool()>& function, std::vector<Systems> requiredSystems)
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
	for (int i = 0; i < SystemsCount; i++) //maybe use the requirementFlag instead of SystemsCount at some point
	{
		unsigned char currentMask = 1 << i;
		if ((requirementFlags & currentMask) >> i == 1)
		{
			Systems currentSystem = (Systems)currentMask;
			std::list<int>& currentSystemSchedule = schedule[currentSystem];
			if (currentSystemSchedule.size() != 0 && functionManager.RunIfReady(currentSystemSchedule.front(), currentAvailableSystem))
			{
				IDsToDelete.push_back(currentSystemSchedule.front());
				//functionManager.Remove(currentSystemSchedule.front());
			}
		}
		currentAvailableSystem <<= 1;
	}

	unsigned char finishedSystems = 0;
	for (int i = 0; i < SystemsCount; i++)
	{
		unsigned char currentMask = 1 << i;
		if ((requirementFlags & currentMask) >> i == 1)
		{
			Systems currentSystem = (Systems)currentMask;
			std::list<int>& currentSystemSchedule = schedule[currentSystem];
			if (currentSystemSchedule.size() == 0)
			{
				finishedSystems |= currentMask;
			}
			else
			{
				bool wasDeleted = false;
				for (int ID : IDsToDelete)
				{
					if (currentSystemSchedule.front() == ID)
					{
						functionManager.Remove(ID);
						currentSystemSchedule.pop_front();
						wasDeleted = true;
						if (currentSystemSchedule.size() == 0)
						{
							finishedSystems |= currentMask;
						}
						else
						{
							functionManager.ResetAvailability(currentSystemSchedule.front());
						}
						break;
					}
				}
				if (!wasDeleted)
				{
					functionManager.ResetAvailability(currentSystemSchedule.front());
				}
			}
		}
	}
	return finishedSystems == requirementFlags;
}