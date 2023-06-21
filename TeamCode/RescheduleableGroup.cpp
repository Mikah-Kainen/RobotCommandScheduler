#include "RescheduleableGroup.h"


void RescheduleableGroup::PreRun()
{
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
}

bool RescheduleableGroup::PostRun(std::vector<unsigned int> packedIDsToDelete)
{
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
				for (unsigned int packedID : packedIDsToDelete)
				{
					if (Unpack(currentSystemSchedule[currentIndices[i]]) == Unpack(packedID))
					{
						//database.Remove(Unpack(packedID));
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
					database.ResetAvailability(Unpack(currentSystemSchedule[currentIndices[i]]));
				}
			}
		}
	}
	shouldInitializeOrHasRestarted = finishedSystems == requirementFlags;
	return shouldInitializeOrHasRestarted;
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
