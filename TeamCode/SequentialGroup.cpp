//#include "../../inc/SchedulerInc/SequentialGroup.h"
#include "SequentialGroup.h"


SequentialGroup::SequentialGroup(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleablesToSchedule)
	:SchedulerBase(GetRequirementFlags(scheduleablesToSchedule), SchedulerTypes::Sequential)
{
	unsigned char requirements = requirementFlags;
	for (std::shared_ptr<FunctionManager::Scheduleable> scheduleable : scheduleablesToSchedule)
	{
		scheduleable->AddRequirement(requirementFlags);
		SchedulerBase::Schedule(scheduleable);
	}
}

//SequentialGroup::SequentialGroup(FunctionManager::Scheduleable** scheduleablesToSchedule, int numberOfScheduleables)
//	:SequentialGroup(std::vector<FunctionManager::Scheduleable*>(numberOfScheduleables, scheduleablesToSchedule[0]))
//{
//}