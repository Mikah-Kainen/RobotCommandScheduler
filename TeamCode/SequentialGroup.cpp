//#include "../../inc/SchedulerInc/SequentialGroup.h"
#include "SequentialGroup.h"


SequentialGroup::SequentialGroup(std::vector<FunctionManager::Scheduleable*> scheduleablesToSchedule)
	:SchedulerBase(GetRequirementFlags(scheduleablesToSchedule), SchedulerTypes::Sequential)
{
	unsigned char requirements = requirementFlags;
	for (FunctionManager::Scheduleable* scheduleable : scheduleablesToSchedule)
	{
		scheduleable->AddRequirement(requirementFlags);
		SchedulerBase::Schedule(scheduleable);
	}
}