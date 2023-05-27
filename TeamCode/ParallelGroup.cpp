//#include "../../inc/SchedulerInc/ParallelGroup.h"
#include "ParallelGroup.h"

//Pretty sure this function is completely useless
//ParallelGroup::ParallelGroup(std::vector<FunctionManager::Scheduleable> functionList, unsigned char requirements)
//	: SchedulerBase(GetRequirementFlag(functionList) | requirements, SchedulerTypes::Parallel)
//{
//	
//}

ParallelGroup::ParallelGroup(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleablesToSchedule)
	: SchedulerBase(GetRequirementFlags(scheduleablesToSchedule), SchedulerTypes::Parallel)
{
	for (std::shared_ptr<FunctionManager::Scheduleable> scheduleable : scheduleablesToSchedule)
	{
		SchedulerBase::Schedule(scheduleable);
	}
}