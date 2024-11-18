//#include "../../inc/Scheduler/ParallelGroup.h"
#include "ParallelGroup.h"

//Pretty sure this function is completely useless
//ParallelGroup::ParallelGroup(std::vector<Scheduleable> functionList, unsigned char requirements)
//	: GroupBase(GetRequirementFlag(functionList) | requirements, SchedulerTypes::Parallel)
//{
//	
//}

ParallelGroup::ParallelGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule)
	: RescheduleableGroup(GetRequirementFlags(scheduleablesToSchedule), GroupTypes::Parallel)
{
	for (std::shared_ptr<Scheduleable> scheduleable : scheduleablesToSchedule)
	{
		GroupBase::Schedule(scheduleable);
	}
}