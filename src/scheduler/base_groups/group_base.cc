#include "../../../inc/scheduler/base_groups/group_base.h"


//Scheduler::ScheduledFunction::ScheduledFunction(std::function<bool()>& backingFunction, unsigned char requirementFlags)
//	:backingFunction{ backingFunction }, RequirementFlags{ requirementFlags } {}

//bool Scheduler::ScheduledFunction::Run()
//{
//	return backingFunction();
//}


unsigned char GroupBase::GetRequirementFlags(std::vector<std::shared_ptr<Schedulable>> schedulables)
{
	unsigned char requirementFlag = 0;
	for (std::shared_ptr<Schedulable> schedulable : schedulables)
	{
		requirementFlag |= schedulable->GetRequirementFlags();
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

#pragma region ProtectedFlagHelperFunctions
bool GroupBase::IsSchedulableBarSet(unsigned int packedID)
{
	return (packedID & BarMask) >= BarMin;
}

bool GroupBase::IsSchedulableShouldInitializeSet(unsigned int packedID)
{
	return packedID & ShouldInitializeMask;
}

bool GroupBase::IsSchedulableEndEarlySet(unsigned int packedID)
{
	return packedID & EndEarlyMask;
}

bool GroupBase::IsGroupEndEarlySet()
{
	return groupFlags & (unsigned char)GroupFlags::EndEarly;
}
#pragma endregion

bool GroupBase::IsFlagSet(unsigned int packedID, unsigned int mask)
{
	return (packedID & mask) != 0;
}

#pragma region DictionaryManager
GroupBase::DictionaryManager::DictionaryManager(std::function<unsigned int(unsigned int)> removeFlagsFunc)
	:removeFlagsFunc{removeFlagsFunc}, schedulableMap {std::unordered_map<unsigned int, std::shared_ptr<Schedulable>>()},
	endBehaviors{ std::unordered_map<unsigned int, std::vector<std::function<void(GroupBase&)>>>() }, nextAvailableID{ 1 }
{
}

GroupBase::DictionaryManager::DictionaryManager()
	:DictionaryManager(std::function<unsigned int(unsigned int)>([=](unsigned int ID) {return ID; })) {}


GroupBase::DictionaryManager::DictionaryManager(const DictionaryManager& copyDictionaryManager)
	: nextAvailableID{ copyDictionaryManager.nextAvailableID }, removeFlagsFunc{copyDictionaryManager.removeFlagsFunc}, schedulableMap {std::unordered_map<unsigned int, std::shared_ptr<Schedulable>>()},
	endBehaviors{ std::unordered_map<unsigned int, std::vector<std::function<void(GroupBase&)>>>() }
{
	for (std::pair<unsigned int, std::shared_ptr<Schedulable>> kvp : copyDictionaryManager.schedulableMap)
	{
		schedulableMap.emplace(kvp.first, kvp.second);
	}

	for (std::pair<unsigned int, std::vector<std::function<void(GroupBase&)>>> kvp : copyDictionaryManager.endBehaviors)
	{
		std::vector<std::function<void(GroupBase&)>> targetBehaviors = std::vector<std::function<void(GroupBase&)>>();
		for (std::function<void(GroupBase&)> behavior : kvp.second)
		{
			targetBehaviors.push_back(behavior);
		}
		endBehaviors.emplace(kvp.first, targetBehaviors);
	}
}

GroupBase::DictionaryManager::~DictionaryManager()
{
	//I should do something in the destructors
}

unsigned int GroupBase::DictionaryManager::Add(std::shared_ptr<Schedulable> scheduledItem)
{
	unsigned int ID = nextAvailableID++;
	schedulableMap.emplace(ID, scheduledItem);
	endBehaviors.emplace(ID, std::vector<std::function<void(GroupBase&)>>());
	return ID;
}

bool GroupBase::DictionaryManager::RunIfReady(unsigned int scheduledID, unsigned char availableSystem)
{
	//bool result = schedulableMap[scheduledID]->RunIfReady(availableSystem);
	bool result = false;
	std::shared_ptr<Schedulable> schedulable = schedulableMap[scheduledID];
	schedulable->AddAvailability(scheduledID);
	if (schedulable->IsReady())
	{
		result = schedulable->RunFSM();
	}
	if (result)
	{
		//do end of function logic
		//throw std::exception("not implemented"); //UNCOMMENT THIS!!
		//Remove(scheduledID); //Don't delete here because I clean up in GroupBase(I could change Cleanup here if I want so maybe think about that)
	}
	return result;
}

void GroupBase::DictionaryManager::Remove(unsigned int ID)
{
	schedulableMap.erase(ID);
	endBehaviors.erase(ID);
	//remove ID from Database
}

void GroupBase::DictionaryManager::ResetAvailability(unsigned int ID)
{
	schedulableMap[ID]->ResetAvailability();
}

void GroupBase::DictionaryManager::SubscribeToEnd(unsigned int targetID, std::function<void(GroupBase&)> endBehavior)
{
	endBehaviors[targetID].push_back(endBehavior);
}

#pragma endregion

#pragma region GroupBase
GroupBase::GroupBase(unsigned char systemFlags, GroupTypes type)
	:Schedulable(systemFlags), initializeFunctions{ std::vector<std::function<void(GroupBase&)>>() }, database{ DictionaryManager() },
	groupID{ NextAvailableGroupID++ }, groupType{ type }, groupFlags{(unsigned char)GroupFlags::None}, requirementFreeSchedulables{std::unordered_set<unsigned int>()},
	schedule{new std::vector<unsigned int>[SystemsCount]}, currentIndices{ new unsigned int[SystemsCount] }
{
	for (int i = 0; i < SystemsCount; i++)
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

GroupBase::GroupBase(std::vector<unsigned char> systemFlags, GroupTypes type)
	:GroupBase(GetRequirementFlag(systemFlags), type) {}

GroupBase::GroupBase(std::vector<Systems> schedulerSystems, GroupTypes type)
	:GroupBase(GetRequirementFlag(schedulerSystems), type) {}


//make it so commands can be scheduled with their parameters passed in(and the FunctionManager properly owns everything)

//add additional scheduling functions to handle scheduling Schedulers
//also think about how conditional functions could work(maybe a placeholder function that is replaced by a Scheduler that handles whichever branch it is currently on)
//also add functionality to remove systems from the scheduler when all the system's commands have been completed, so that the parent scheduler can start scheduling things with that system(but don't remove systems from the original scheduler!)
//make different types of schedulers or have information in the schedulers which decides whether a system should be removed or its default functions should be run when the scheduler runs out of scheduled functions for that system
unsigned int GroupBase::Schedule(std::shared_ptr<Schedulable> schedulable)
{
	unsigned char schedulableRequirementFlags = schedulable->GetRequirementFlags();
	unsigned int unpackedID = database.Add(schedulable);
	unsigned int packedID = Pack(unpackedID) | ShouldInitializeMask;
	if (schedulableRequirementFlags == 0)
	{
		requirementFreeSchedulables.insert(packedID);
	}
	else
	{
		for (unsigned int i = 0; i < SystemsCount; i++)
		{
			unsigned char currentMask = 1 << i;
			if ((schedulableRequirementFlags & currentMask) >> i == 1)
			{
				schedule[i].push_back(packedID);
				//	Systems currentSystem = (Systems)currentMask;
				//	schedule[currentSystem].push_back(newID);
			}
		}
	}
	//Maybe put stuff here
	return packedID;
}

unsigned int GroupBase::Schedule(std::function<bool()> function, unsigned char requirementFlags)
{
	return Schedule(std::make_shared<Schedulable>(function, requirementFlags));
}

unsigned int GroupBase::Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems)
{
	return Schedule(function, GetRequirementFlag(requiredSystems));
}

void GroupBase::ReplaceIDUnpacked(unsigned int unpackedID, unsigned int newPackedID)
{
	for (unsigned int currentPackedID : requirementFreeSchedulables)
	{
		if (Unpack(currentPackedID) == unpackedID)
		{
			requirementFreeSchedulables.erase(currentPackedID);
			requirementFreeSchedulables.insert(newPackedID);
			break;
		}
	}
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

GroupBase::GroupBase(const GroupBase& copyGroupBase)
	: Schedulable(copyGroupBase.requirementFlags), initializeFunctions{ std::vector<std::function<void(GroupBase&)>>() },
	database{ copyGroupBase.database }, groupType{ copyGroupBase.groupType }, groupID{ NextAvailableGroupID++ }, groupFlags{copyGroupBase.groupFlags},
	requirementFreeSchedulables{std::unordered_set<unsigned int>()}, schedule{new std::vector<unsigned int>[SystemsCount]}, 
	currentIndices{new unsigned int[SystemsCount]}
{
	for (int i = 0; i < copyGroupBase.initializeFunctions.size(); i++)
	{
		initializeFunctions.push_back(copyGroupBase.initializeFunctions[i]);
	}

	for (unsigned int packedID : copyGroupBase.requirementFreeSchedulables)
	{
		requirementFreeSchedulables.insert(packedID);
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

#pragma region PublicFlagHelperFunctions
void GroupBase::AddBar(unsigned int packedID)
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

void GroupBase::SetSchedulableEndEarly(unsigned int packedID, bool value)
{
	ReplaceID(packedID, (packedID & ~EndEarlyMask) + EndEarlyMask * value);
}

void GroupBase::SetSchedulableShouldInitialize(unsigned int packedID, bool value)
{
	ReplaceID(packedID, (packedID & ~ShouldInitializeMask) + ShouldInitializeMask * value);
}

void GroupBase::SetGroupEndEarly(bool value)
{
	groupFlags = (groupFlags & !(unsigned char)GroupFlags::EndEarly) + (unsigned char)GroupFlags::EndEarly * value;
}
#pragma endregion

void GroupBase::ReplaceID(unsigned int oldPackedID, unsigned int newPackedID)
{
	ReplaceIDUnpacked(Unpack(oldPackedID), newPackedID);
}

void GroupBase::SubscribeToEnd(unsigned int targetPackedID, std::function<void(GroupBase&)> endBehavior)
{
	database.SubscribeToEnd(Unpack(targetPackedID), endBehavior);
}

GroupBase::Behaviors GroupBase::GetNextBehavior(int packedID)
{
	if (IsSchedulableEndEarlySet(packedID))
	{
		return Behaviors::EndEarly;
	}
	else if (IsSchedulableBarSet(packedID))
	{
		return Behaviors::Bar;
	}
	else if (IsSchedulableShouldInitializeSet(packedID))
	{
		return Behaviors::Initialize;
	}
	else
	{
		return Behaviors::Run;
	}
}

void GroupBase::Update()
{
	//This is probably unnecessary
}

//if (IsFlagSet(packedID, EndEarlyMask))
//{
//	//functionManager.EndEarly(Unpack(currentSystemSchedule[0])); //Add EndEarly to function Manager
//	//throw std::exception("Add EndEarly function to FunctionManger!!");
//	std::cout << "Add EndEarly function to GroupBase!!" << std::endl;
//	while (true);
//}
//else if (!IsFlagSet(packedID, BarMask)) //I should change this check after I add in the current node array for the schedule lists
//{
//	if (IsFlagSet(packedID, ShouldInitializeMask))
//	{
//		database.schedulableMap[Unpack(packedID)]->Initialize();
//		ReplaceID(packedID, packedID & ~ShouldInitializeMask);
//	}
//	else if (database.RunIfReady(Unpack(packedID), currentAvailableSystem))
//	{
//		packedIDsToDelete.push_back(packedID);
//		for (int x = 0; x < database.endBehaviors[Unpack(packedID)].size(); x++)
//		{
//			database.endBehaviors[Unpack(packedID)][x](*this);
//			//database.endBehaviors[Unpack(currentID)].erase(database.endBehaviors[Unpack(currentID)].begin() + x);
//		}

//		currentIndices[i]++;
//		for (int x = 0; x < i; x++)
//		{
//			if (currentIndices[x] < schedule[x].size() && schedule[x][currentIndices[x]] == packedID)
//			{
//				currentIndices[x] ++;
//			}
//		}
//		Remove(packedID);
//		//functionManager.Remove(currentSystemSchedule.front());
//	}
//}
//else
//{
//	//currentSystem = Systems::None;
//	//run default function for system
//}

bool GroupBase::Run()
{
	//std::cout << "Run Ran\n";
	//for (int i = 0; i < SystemsCount; i ++) //Loop for initializing schedulables
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

	//if (shouldInitializeOrHasRestarted)
	//{
	//	for (int i = 0; i < SystemsCount; i++)
	//	{
	//		currentIndices[i] = 0;
	//	}
	//	for (std::function<void(GroupBase&)> func : initializeFunctions)
	//	{
	//		func(*this);
	//	}
	//	shouldInitializeOrHasRestarted = false;
	//}
	
	if (groupFlags & (unsigned char)GroupFlags::EndEarly)
	{
		return true;
	}

	if (requirementFreeSchedulables.size() > 0)
	{
		std::vector<unsigned int> noRequirementPackedIDsToDelete = std::vector<unsigned int>();
		std::vector<std::tuple<unsigned int, unsigned int>> noRequirementPackedIDsToReplace = std::vector<std::tuple<unsigned int, unsigned int>>();
		for (unsigned int packedID : requirementFreeSchedulables)
		{
			unsigned int unpackedID = Unpack(packedID);
			bool finished = false;
			GroupBase::Behaviors nextBehavior = GetNextBehavior(packedID);
			switch (nextBehavior)
			{
			case Behaviors::EndEarly:
				finished = true;
				break;

			case Behaviors::Bar:
				//run default function for system
				break;

			case Behaviors::Initialize:
				database.schedulableMap[unpackedID]->Initialize();
				noRequirementPackedIDsToReplace.push_back(std::tuple<unsigned int, unsigned int>(packedID, packedID & ~ShouldInitializeMask));
				break;

			case Behaviors::Run:
				finished = database.schedulableMap[unpackedID]->RunFSM();
					//Doesn't call RunIfReady because Systems::None has no requirements
					//This will break if I ever add more functionality to RunIfReady
				break;
			}
			if (finished)
			{
				for (int x = 0; x < database.endBehaviors[unpackedID].size(); x++)
				{
					database.endBehaviors[unpackedID][x](*this);
					//database.endBehaviors[Unpack(currentID)].erase(database.endBehaviors[Unpack(currentID)].begin() + x);
				}

				noRequirementPackedIDsToDelete.push_back(packedID);
				//functionManager.Remove(currentSystemSchedule.front());
			}
		}
		for (std::tuple<unsigned int, unsigned int> tuple : noRequirementPackedIDsToReplace)
		{
			ReplaceID(std::get<0>(tuple), std::get<1>(tuple));
		}
		for (unsigned int IDToDelete : noRequirementPackedIDsToDelete)
		{
			Remove(IDToDelete);
			requirementFreeSchedulables.erase(IDToDelete);
		}
	}
	else
	{
		//run default function for Systems::None
	}

	unsigned char currentAvailableSystem = (unsigned char)0x1;
	std::vector<unsigned int> packedIDsToDelete = std::vector<unsigned int>();
	for (int i = 0; i < SystemsCount; i++) //maybe use the requirementFlag instead of SystemsCount at some point
	{
		unsigned char currentMask = 1 << i;
		if ((requirementFlags & currentMask) >> i == 1) //I should keep checking if a system is part of the Group's requirements because it will save unnecessary function calls when I start calling default functions
		{
			Systems currentSystem = (Systems)currentMask;
			std::vector<unsigned int>& currentSystemSchedule = schedule[i];
			if (currentIndices[i] < currentSystemSchedule.size())
			{
				unsigned int currentPackedID = currentSystemSchedule[currentIndices[i]];
				std::shared_ptr<Schedulable> currentSchedulable = database.schedulableMap[Unpack(currentPackedID)];
				currentSchedulable->AddAvailability(currentAvailableSystem);
				if (currentSchedulable->IsReady())
				{
					bool finished = false;
					GroupBase::Behaviors nextBehavior = GetNextBehavior(currentPackedID);
					switch (nextBehavior)
					{
					case Behaviors::EndEarly:
						finished = true;
						break;

					case Behaviors::Bar:
						//run default function for system
						break;

					case Behaviors::Initialize:
						database.schedulableMap[Unpack(currentPackedID)]->Initialize(); //was InitializeFSM
						SetSchedulableShouldInitialize(currentPackedID, false);
						break;

					case Behaviors::Run:
						finished = currentSchedulable->RunFSM();
						break;
					}
					if (finished)
					{
						packedIDsToDelete.push_back(currentPackedID);
						for (int x = 0; x < database.endBehaviors[Unpack(currentPackedID)].size(); x++)
						{
							database.endBehaviors[Unpack(currentPackedID)][x](*this);
							//database.endBehaviors[Unpack(currentID)].erase(database.endBehaviors[Unpack(currentID)].begin() + x);
						}

						currentIndices[i]++;
						for (int x = 0; x < i; x++)
						{
							if (currentIndices[x] < schedule[x].size() && schedule[x][currentIndices[x]] == currentPackedID)
							{
								currentIndices[x] ++;
							}
						}
						Remove(currentPackedID);
						//functionManager.Remove(currentSystemSchedule.front());
					}
				}
			}
			else
			{
				//run default function for system
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
			std::vector<unsigned int>& currentSystemSchedule = schedule[i];
			if (currentIndices[i] >= currentSystemSchedule.size())
			{
				finishedSystems |= currentMask;
			}
			else
			{
				database.ResetAvailability(Unpack(currentSystemSchedule[currentIndices[i]]));
			}
		}
	}

	return Return(finishedSystems == requirementFlags & requirementFreeSchedulables.size() == 0);
	//unsigned char finishedSystems = 0;
	//for (unsigned int i = 0; i < SystemsCount; i++)
	//{
	//	unsigned char currentMask = 1 << i;
	//	if ((requirementFlags & currentMask) >> i == 1)
	//	{
	//		Systems currentSystem = (Systems)currentMask;
	//		std::vector<unsigned int>& currentSystemSchedule = schedule[i];
	//		if (currentIndices[i] >= currentSystemSchedule.size())
	//		{
	//			finishedSystems |= currentMask;
	//		}
	//		else
	//		{
	//			bool shouldReset = true;
	//			for (unsigned int packedID : IDsToDelete)
	//			{
	//				if (Unpack(currentSystemSchedule[currentIndices[i]]) == Unpack(packedID))
	//				{
	//					//database.Remove(Unpack(packedID));
	//					//Pop_Front<unsigned int>(currentSystemSchedule);
	//					currentIndices[i]++;
	//					if (currentIndices[i] >= currentSystemSchedule.size())
	//					{
	//						finishedSystems |= currentMask;
	//						shouldReset = false;
	//					}
	//					break;
	//				}
	//			}
	//			if (shouldReset)
	//			{
	//				database.ResetAvailability(Unpack(currentSystemSchedule[currentIndices[i]]));
	//			}
	//		}
	//	}
	//}
	//shouldInitializeOrHasRestarted = finishedSystems == requirementFlags;
	//return shouldInitializeOrHasRestarted;
}

void GroupBase::Initialize()
{
	Schedulable::Initialize();
}
#pragma endregion