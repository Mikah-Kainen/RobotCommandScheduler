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

//bool GroupBase::IsBarSet(unsigned int packedID)
//{
//	return (packedID & BarMask) >= BarMin;
//}

bool GroupBase::IsFlagSet(unsigned int packedID, unsigned int mask)
{
	return (packedID & mask) != 0;
}

GroupBase::GroupBase(unsigned char systemFlags, SchedulerTypes type)
	:Scheduleable(systemFlags), initializeFunctions{std::vector<std::function<void(GroupBase&)>>()}, functionManager {FunctionManager()}, 
	 schedulerID{ NextAvailableSchedulerID++ }, schedulerType{ type }, schedule{ new std::vector<unsigned int>[SystemsCount] }, 
	 currentIndices{ new unsigned int[SystemsCount] }
{
	for (int i = 0; i < SystemsCount; i ++)
	{
		currentIndices[i] = 0;
	}
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
unsigned int GroupBase::Schedule(std::shared_ptr<Scheduleable> scheduleable)
{
	unsigned char scheduleableRequirementFlags = scheduleable->GetRequirementFlags();
	unsigned int unpackedID = functionManager.AddToDatabase(scheduleable);
	unsigned int packedID = Pack(unpackedID) /*| ShouldInitializeMask*/;
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

unsigned int GroupBase::Schedule(std::function<bool()> function, unsigned char requirementFlags)
{
	return Schedule(std::make_shared<Scheduleable>(function, requirementFlags));
}

unsigned int GroupBase::Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems)
{
	return Schedule(function, GetRequirementFlag(requiredSystems));
}

void GroupBase::ReplaceIDUnpacked(unsigned int unpackedID, unsigned int newPackedID)
{
	for (int i = 0; i < SystemsCount; i++)
	{
		std::vector<unsigned int>& currentSystemSchedule = schedule[i];
		for (int x = 0; x < currentSystemSchedule.size(); x++)
		{
			if (Unpack(currentSystemSchedule[x]) == unpackedID)
			{
				//currentSystemSchedule[x] = newID;
				currentSystemSchedule[x] = newPackedID;
				break;
			}
		}
	}
}

//void GroupBase::SetShouldInitialize(unsigned int packedID)
//{
//	int unpackedID = Unpack(packedID);
//	int newID = packedID | ShouldInitializeMask;
//	for (int i = 0; i < SystemsCount; i++)
//	{
//		std::vector<unsigned int>& currentSystemSchedule = schedule[i];
//		for (int x = 0; x < currentSystemSchedule.size(); x++)
//		{
//			if (Unpack(currentSystemSchedule[x]) == packedID)
//			{
//				currentSystemSchedule[x] = newID;
//				break;
//			}
//		}
//	}
//}
//
//void GroupBase::SetEndEarly(unsigned int packedID)
//{
//	int unpackedID = Unpack(packedID);
//	int newID = packedID | EndEarlyMask;
//	for (int i = 0; i < SystemsCount; i++)
//	{
//		std::vector<unsigned int>& currentSystemSchedule = schedule[i];
//		for (int x = 0; x < currentSystemSchedule.size(); x++)
//		{
//			if (Unpack(currentSystemSchedule[x]) == packedID)
//			{
//				currentSystemSchedule[x] = newID;
//				break;
//			}
//		}
//	}
//}


GroupBase::GroupBase(const GroupBase& copyGroupBase)
	: Scheduleable(copyGroupBase.requirementFlags), initializeFunctions{std::vector<std::function<void(GroupBase&)>>()}, 
	functionManager{copyGroupBase.functionManager},schedulerType{ copyGroupBase.schedulerType }, schedulerID{ copyGroupBase.schedulerID }, 
	schedule{ new std::vector<unsigned int>[SystemsCount] }, currentIndices{ new unsigned int[SystemsCount] }, shouldInitializeOrHasRestarted{copyGroupBase.shouldInitializeOrHasRestarted}
{
	for (int i = 0; i < copyGroupBase.initializeFunctions.size(); i ++)
	{
		initializeFunctions.push_back(copyGroupBase.initializeFunctions[i]);
	}

	for (int i = 0; i < SystemsCount; i++)
	{
		std::vector<unsigned int>& currentSystemSchedule = copyGroupBase.schedule[i];
		for (unsigned int packedID : currentSystemSchedule)
		{
			schedule[i].push_back(packedID);
		}

		currentIndices[i] = copyGroupBase.currentIndices[i];
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

void GroupBase::SetBar(unsigned int packedID)
{
	unsigned int BarBits = ((packedID << 1) + BarMin) & BarMask;
	unsigned int newPackedID = packedID | BarBits;
	ReplaceID(packedID, newPackedID);
}

void GroupBase::RemoveBar(unsigned int packedID)
{
	unsigned int BarBits = (packedID & BarMask) >> 1;
	unsigned int newPackedID = packedID & (~BarMask + BarBits);
	ReplaceID(packedID, newPackedID);
}

void GroupBase::ReplaceID(unsigned int oldPackedID, unsigned int newPackedID)
{
	ReplaceIDUnpacked(Unpack(oldPackedID), newPackedID);
}

void GroupBase::SubscribeToEnd(unsigned int targetPackedID, std::function<void()> endBehavior)
{
	functionManager.SubscribeToEnd(Unpack(targetPackedID), endBehavior);
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
	//for (int i = 0; i < SystemsCount; i ++) //Loop for initializing scheduleables
	//{
	//	unsigned char currentMask = 1 << i;
	//	if ((requirementFlags & currentMask) >> i == 1) //I should keep checking if a system is part of the Group's requirements because it will save unnecessary function calls when I start calling default functions
	//	{
	//		Systems currentSystem = (Systems)currentMask;
	//		std::vector<unsigned int>& currentSystemSchedule = schedule[i];
	//		if (currentIndices[i] < currentSystemSchedule.size())
	//		{
	//			unsigned int currentID = currentSystemSchedule[currentIndices[i]];
	//			if (IsFlagSet(currentID, ShouldInitializeMask))
	//			{
	//				Initialize();
	//				ReplaceID(currentID, currentID & ~ShouldInitializeMask);
	//			}
	//		}
	//	}
	//}
	if (shouldInitializeOrHasRestarted)
	{
		for (int i = 0; i < SystemsCount; i++)
		{
			currentIndices[i] = 0;
		}
		for (std::function<void(GroupBase&)> func : initializeFunctions)
		{
			func(*this);
		}
		shouldInitializeOrHasRestarted = false;
	}

	for (int i = 0; i < SystemsCount; i++) //maybe use the requirementFlag instead of SystemsCount at some point
	{
		unsigned char currentMask = 1 << i;
		if ((requirementFlags & currentMask) >> i == 1) //I should keep checking if a system is part of the Group's requirements because it will save unnecessary function calls when I start calling default functions
		{
			Systems currentSystem = (Systems)currentMask;
			std::vector<unsigned int>& currentSystemSchedule = schedule[i];
			if (currentIndices[i] < currentSystemSchedule.size())
			{
				unsigned int currentID = currentSystemSchedule[currentIndices[i]];
				if (IsFlagSet(currentID, EndEarlyMask))
				{
					//functionManager.EndEarly(Unpack(currentSystemSchedule[0])); //Add EndEarly to function Manager
					throw std::exception("Add EndEarly function to FunctionManger!!");
				}
				else if (!IsFlagSet(currentID, BarMask)) //I should change this check after I add in the current node array for the schedule lists
				{
					if (functionManager.RunIfReady(Unpack(currentID), currentAvailableSystem))
					{
						IDsToDelete.push_back(currentID);
						for (int x = 0; x < functionManager.endBehaviors[Unpack(currentID)].size(); x ++)
						{
							functionManager.endBehaviors[Unpack(currentID)][x]();
							functionManager.endBehaviors[Unpack(currentID)].erase(functionManager.endBehaviors[Unpack(currentID)].begin() + x);
						}
						//functionManager.Remove(currentSystemSchedule.front());
					}
				}
				else
				{
					//currentSystem = Systems::None;
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
			if (currentIndices[i] >= currentSystemSchedule.size())
			{
				finishedSystems |= currentMask;
			}
			else
			{
				bool shouldReset = true;
				for (unsigned int packedID : IDsToDelete)
				{
					if (Unpack(currentSystemSchedule[currentIndices[i]]) == Unpack(packedID))
					{
						//functionManager.Remove(Unpack(packedID));
						//Pop_Front<unsigned int>(currentSystemSchedule);
						currentIndices[i]++;
						if (currentIndices[i] >= currentSystemSchedule.size())
						{
							finishedSystems |= currentMask;
							shouldReset = false;
						}
						break;
					}
				}
				if (shouldReset)
				{
					functionManager.ResetAvailability(Unpack(currentSystemSchedule[currentIndices[i]]));
				}
			}
		}
	}
	shouldInitializeOrHasRestarted = finishedSystems == requirementFlags;
	return shouldInitializeOrHasRestarted;
}