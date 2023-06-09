//#include "../../inc/SchedulerInc/SequentialGroup.h"
#include "SequentialGroup.h"


SequentialGroup::SequentialGroup(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleablesToSchedule)
	:GroupBase(GetRequirementFlags(scheduleablesToSchedule), SchedulerTypes::Sequential)
{
	for (std::shared_ptr<FunctionManager::Scheduleable> scheduleable : scheduleablesToSchedule)
	{
		scheduleable->AddRequirement(requirementFlags);
		GroupBase::Schedule(scheduleable);
	}
}

//SequentialGroup::SequentialGroup(FunctionManager::Scheduleable** scheduleablesToSchedule, int numberOfScheduleables)
//	:SequentialGroup(std::vector<FunctionManager::Scheduleable*>(numberOfScheduleables, scheduleablesToSchedule[0]))
//{
//}