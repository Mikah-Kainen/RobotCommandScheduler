//#include "../../inc/SchedulerInc/RescheduleableGroup.h"
#include "RescheduleableGroup.h"

void RescheduleableGroup::Initialize()
{
	if (shouldInitializeOrHasRestarted)
	{
		InitializeBody();
	}
}

void RescheduleableGroup::Remove(unsigned int packedID)
{
	//RescheduleableGroups don't remove anything
}

bool RescheduleableGroup::Return(bool isFinished)
{
	shouldInitializeOrHasRestarted = isFinished;
	return isFinished;
}

void RescheduleableGroup::InitializeBody()
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
	:GroupBase(copyRescheduleableGroup)
{

}
