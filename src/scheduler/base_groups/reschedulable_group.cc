#include "../../../inc/scheduler/base_groups/reschedulable_group.h"

//void ReschedulableGroup::InitializeGroup()
//{
//	if (shouldInitializeOrHasRestarted)
//	{
//		InitializeBody();
//	}
//}

void ReschedulableGroup::Remove(unsigned int packedID)
{
	if (requirementFreeSchedulables.count(packedID) > 0)
	{
		initializeRequirementFreeSchedulables.insert(packedID);
	}
	//ReschedulableGroups don't remove anything
}

bool ReschedulableGroup::Return(bool isFinished)
{
	return isFinished;
}

//void ReschedulableGroup::InitializeBody()
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
//	Initialize();
//}

ReschedulableGroup::ReschedulableGroup(unsigned char systemFlags, GroupTypes type)
	:GroupBase(systemFlags, type), initializeRequirementFreeSchedulables{ std::unordered_set<unsigned int>() }
{
}

ReschedulableGroup::ReschedulableGroup(std::vector<unsigned char> systemFlags, GroupTypes type)
	:GroupBase(systemFlags, type), initializeRequirementFreeSchedulables{ std::unordered_set<unsigned int>() }
{
}

ReschedulableGroup::ReschedulableGroup(std::vector<Systems> schedulerSystems, GroupTypes type)
	:GroupBase(schedulerSystems, type), initializeRequirementFreeSchedulables{ std::unordered_set<unsigned int>() }
{
}

ReschedulableGroup::ReschedulableGroup(const ReschedulableGroup& copyReschedulableGroup)
	:GroupBase(copyReschedulableGroup), initializeRequirementFreeSchedulables{ std::unordered_set<unsigned int>() }/*, shouldInitializeOrHasRestarted{ copyReschedulableGroup.shouldInitializeOrHasRestarted }*/
{
	for (unsigned int packedID : copyReschedulableGroup.initializeRequirementFreeSchedulables)
	{
		initializeRequirementFreeSchedulables.insert(packedID);
	}
}

void ReschedulableGroup::Initialize()
{
	for (unsigned int packedID : requirementFreeSchedulables)
	{
		initializeRequirementFreeSchedulables.insert(packedID);
	}
	requirementFreeSchedulables.clear();
	for (unsigned int packedID : initializeRequirementFreeSchedulables)
	{
		requirementFreeSchedulables.insert(packedID | ShouldInitializeMask);
	}
	initializeRequirementFreeSchedulables.clear();
	for (int i = 0; i < SystemsCount; i ++)
	{
		currentIndices[i] = 0;

		std::vector<unsigned int>& currentSystemSchedule = schedule[i];
		for (int x = 0; x < currentSystemSchedule.size(); x ++)
		{
			currentSystemSchedule[x] |= ShouldInitializeMask;
		}
	}
	for (std::function<void(GroupBase&)> func : initializeFunctions)
	{
		func(*this);
	}
	for (std::pair<unsigned int, std::shared_ptr<Schedulable>> kvp : database.schedulableMap)
	{
		kvp.second->ResetAvailability();
	}
	GroupBase::Initialize();
}