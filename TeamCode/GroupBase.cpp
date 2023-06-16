//#include "../../inc/SchedulerInc/GroupBase.h"
#include "GroupBase.h"

//Scheduler::ScheduledFunction::ScheduledFunction(std::function<bool()>& backingFunction, unsigned char requirementFlags)
//	:backingFunction{ backingFunction }, RequirementFlags{ requirementFlags } {}

//bool Scheduler::ScheduledFunction::Run()
//{
//	return backingFunction();
//}


unsigned char GroupBase::GetRequirementFlags(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleables)
{
	unsigned char requirementFlag = 0;
	for (std::shared_ptr<FunctionManager::Scheduleable> scheduleable : scheduleables)
	{
		requirementFlag |= scheduleable->GetRequirementFlags();
	}
	return requirementFlag;
}

unsigned int GroupBase::Unpack(unsigned int packedID)
{
	return (packedID & UnpackMask) >> BarBitCount;
}

unsigned int GroupBase::Pack(unsigned int unpackedID)
{
	return (unpackedID << BarBitCount) & UnpackMask;
}

bool GroupBase::IsBarSet(unsigned int packedID)
{
	return (packedID & BarMask) >= BarMin;
}

bool GroupBase::IsEndEarlySet(unsigned int packedID)
{
	return packedID > EndEarlyMin; //this works becaue EndEarly bits are the last bits in the ID and IDs start at BarMin not 0
}

GroupBase::GroupBase(unsigned char systemFlags, SchedulerTypes type)
	:Scheduleable(/*std::function<bool()>([&]() {return Run(); }),*//*std::function<bool()>(std::bind(&GroupBase::Run, this)*/ systemFlags), functionManager{ FunctionManager() }, currentlyRunningSystems{ 0 },
	schedulerID{ NextAvailableSchedulerID++ }, schedulerType{ type }, schedule{ new std::vector<unsigned int>[SystemsCount] }
	//I don't use std::bind because it causes a copy anyway when passed to the next function despite being passed as a reference(I think)
{

	//List should be automatically initialized
	//schedule = std::unordered_map<Systems, std::list<int>>();
	//for (int i = 0; i < SystemsCount; i++)
	//{
	//	unsigned char currentMask = 1 << i;
	//	if ((systemFlags & currentMask) >> i == 1)
	//	{
	//		schedule[i] = std::list<int>();
	//		//	Systems currentSystem = (Systems)currentMask;
	//		//	schedule.emplace(currentSystem, std::list<int>());
	//	}
	//}

	//NextAvailableSchedulerID++;
}

GroupBase::GroupBase(std::vector<unsigned char> systemFlags, SchedulerTypes type)
	:GroupBase(GetRequirementFlag(systemFlags), type) {}

GroupBase::GroupBase(std::vector<Systems> schedulerSystems, SchedulerTypes type)
	:GroupBase(GetRequirementFlag(schedulerSystems), type) {}


//make it so commands can be scheduled with their parameters passed in(and the FunctionManager properly owns everything)

//add additional scheduling functions to handle scheduling Schedulers
//also think about how conditional functions could work(maybe a placeholder function that is replaced by a Scheduler that handles whichever branch it is currently on)
//also add functionality to remove systems from the scheduler when all the system's commands have been completed, so that the parent scheduler can start scheduling things with that system(but don't remove systems from the original scheduler!)
//make different types of schedulers or have information in the schedulers which decides whether a system should be removed or its default functions should be run when the scheduler runs out of scheduled functions for that system
int GroupBase::Schedule(std::shared_ptr<Scheduleable> scheduleable)
{
	unsigned char scheduleableRequirementFlags = scheduleable->GetRequirementFlags();
	unsigned int unpackedID = functionManager.AddToDatabase(scheduleable);
	unsigned int packedID = Pack(unpackedID);
	for (unsigned int i = 0; i < SystemsCount; i++)
	{
		unsigned char currentMask = 1 << i;
		if ((scheduleableRequirementFlags & currentMask) >> i == 1)
		{
			schedule[i].push_back(packedID);
			//	Systems currentSystem = (Systems)currentMask;
			//	schedule[currentSystem].push_back(newID);
		}
	}
	//Definitely put stuff here
	return packedID;
}

int GroupBase::Schedule(std::function<bool()> function, unsigned char requirementFlags)
{
	return Schedule(std::make_shared<Scheduleable>(function, requirementFlags));
}

int GroupBase::Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems)
{
	return Schedule(function, GetRequirementFlag(requiredSystems));
}


void GroupBase::SetBar(unsigned int packedID)
{
	int unpackedID = Unpack(packedID);
	int BarBits = ((packedID << 1) + BarMin) & BarMask;
	int newID = packedID | BarBits;
	for (int i = 0; i < SystemsCount; i++)
	{
		std::vector<unsigned int>& currentSystemSchedule = schedule[i];
		for (int x = 0; x < currentSystemSchedule.size(); x++)
		{
			if (Unpack(currentSystemSchedule[x]) == unpackedID)
			{
				currentSystemSchedule[x] = newID;
				break;
			}
		}
	}
}

void GroupBase::RemoveBar(unsigned int packedID)
{
	int unpackedID = Unpack(packedID);
	int BarBits = (packedID & BarMask) >> 1;
	int newID = packedID & (~BarMask + BarBits);
	for (int i = 0; i < SystemsCount; i++)
	{
		std::vector<unsigned int>& currentSystemSchedule = schedule[i];
		for (int x = 0; x < currentSystemSchedule.size(); x++)
		{
			if (Unpack(currentSystemSchedule[x]) == unpackedID)
			{
				currentSystemSchedule[x] = newID;
				break;
			}
		}
	}
}

void GroupBase::SetEndEarly(unsigned int packedID)
{
	int unpackedID = Unpack(packedID);
	int newID = packedID | EndEarlyMin;
	for (int i = 0; i < SystemsCount; i++)
	{
		std::vector<unsigned int>& currentSystemSchedule = schedule[i];
		for (int x = 0; x < currentSystemSchedule.size(); x++)
		{
			if (Unpack(currentSystemSchedule[x]) == packedID)
			{
				currentSystemSchedule[x] = newID;
				break;
			}
		}
	}
}

void GroupBase::Subscribe(unsigned int targetID, std::function<void()> endBehavior)
{
	functionManager.Subscribe(targetID, endBehavior);
}


GroupBase::GroupBase(const GroupBase& copyGroupBase)
	: Scheduleable(copyGroupBase.requirementFlags), functionManager{ copyGroupBase.functionManager }, schedulerType{ copyGroupBase.schedulerType },
	currentlyRunningSystems{ copyGroupBase.currentlyRunningSystems }, schedulerID{ copyGroupBase.schedulerID }, schedule{ new std::vector<unsigned int>[SystemsCount] }
{
	for (unsigned int i = 0; i < SystemsCount; i++)
	{
		std::vector<unsigned int>& currentSystemSchedule = copyGroupBase.schedule[i];
		for (unsigned int packedID : currentSystemSchedule)
		{
			schedule[i].push_back(packedID);
		}
	}
	//schedule = std::unordered_map<Systems, std::list<int>>();
	//for (std::pair<Systems, std::list<int>> kvp : copyGroupBase.schedule)
	//{
	//	std::list<int> copyList = std::list<int>();
	//	for (int number : kvp.second)
	//	{
	//		copyList.push_back(number);
	//	}
	//	schedule.emplace(kvp.first, copyList);
	//}
}

void GroupBase::Update()
{
	//This is probably unnecessary
}

bool GroupBase::Run()
{
	//std::cout << "Run Ran\n";
	unsigned char currentAvailableSystem = (unsigned char)0x1;
	std::vector<unsigned int> IDsToDelete = std::vector<unsigned int>();
	for (unsigned int i = 0; i < SystemsCount; i++) //maybe use the requirementFlag instead of SystemsCount at some point
	{
		unsigned char currentMask = 1 << i;
		if ((requirementFlags & currentMask) >> i == 1) //I should keep checking if a system is part of the Group's requirements because it will save unnecessary function calls when I start calling default functions
		{
			Systems currentSystem = (Systems)currentMask;
			std::vector<unsigned int>& currentSystemSchedule = schedule[i];
			if (currentSystemSchedule.size() != 0)
			{
				if (IsEndEarlySet(currentSystemSchedule[0]))
				{
					//functionManager.EndEarly(Unpack(currentSystemSchedule[0])); //Add EndEarly to function Manager
					throw std::exception("Add EndEarly function to FunctionManger!!");
				}
				else if (!IsBarSet(currentSystemSchedule[0])) //I should change this check after I add in the current node array for the schedule lists
				{
					if (functionManager.RunIfReady(Unpack(currentSystemSchedule[0]), currentAvailableSystem))
					{
						IDsToDelete.push_back(currentSystemSchedule[0]);
						//functionManager.Remove(currentSystemSchedule.front());
					}
				}
				else
				{
					//run default function for system
				}
			}
			else
			{
				//run default function for system
			}
		}
		currentAvailableSystem <<= 1;
	}

	//I should change this to not actually remove the scheduleables from the schedule in case I want to schedule the same scheduleable again
	unsigned char finishedSystems = 0;
	for (unsigned int i = 0; i < SystemsCount; i++)
	{
		unsigned char currentMask = 1 << i;
		if ((requirementFlags & currentMask) >> i == 1)
		{
			Systems currentSystem = (Systems)currentMask;
			std::vector<unsigned int>& currentSystemSchedule = schedule[i];
			if (currentSystemSchedule.size() == 0)
			{
				finishedSystems |= currentMask;
			}
			else
			{
				bool wasDeleted = false;
				for (unsigned int packedID : IDsToDelete)
				{
					if (currentSystemSchedule[0] == packedID)
					{
						functionManager.Remove(Unpack(packedID));
						Pop_Front<unsigned int>(currentSystemSchedule);
						wasDeleted = true;
						if (currentSystemSchedule.size() == 0)
						{
							finishedSystems |= currentMask;
						}
						else
						{
							functionManager.ResetAvailability(Unpack(currentSystemSchedule[0]));
						}
						break;
					}
				}
				if (!wasDeleted)
				{
					functionManager.ResetAvailability(Unpack(currentSystemSchedule[0]));
				}
			}
		}
	}
	return finishedSystems == requirementFlags;
}