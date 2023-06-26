//#include "../../inc/SchedulerInc/RescheduleableGroup.h"
#include "RescheduleableGroup.h"

//void RescheduleableGroup::InitializeGroup()
//{
//	if (shouldInitializeOrHasRestarted)
//	{
//		InitializeBody();
//	}
//}

void RescheduleableGroup::Remove(unsigned int packedID)
{
	//RescheduleableGroups don't remove anything
}

bool RescheduleableGroup::Return(bool isFinished)
{
	return isFinished;
}

//void RescheduleableGroup::InitializeBody()
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

RescheduleableGroup::RescheduleableGroup(unsigned char systemFlags, SchedulerTypes type)
	:GroupBase(systemFlags, type)
{

}

RescheduleableGroup::RescheduleableGroup(std::vector<unsigned char> systemFlags, SchedulerTypes type)
	:GroupBase(systemFlags, type)
{

}

RescheduleableGroup::RescheduleableGroup(std::vector<Systems> schedulerSystems, SchedulerTypes type)
	:GroupBase(schedulerSystems, type)
{

}

RescheduleableGroup::RescheduleableGroup(const RescheduleableGroup& copyRescheduleableGroup)
	:GroupBase(copyRescheduleableGroup)/*, shouldInitializeOrHasRestarted{ copyRescheduleableGroup.shouldInitializeOrHasRestarted }*/
{

}

bool RescheduleableGroup::Initialize()
{
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
	return GroupBase::Initialize();
}