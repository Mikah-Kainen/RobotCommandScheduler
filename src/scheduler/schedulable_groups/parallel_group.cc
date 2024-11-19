#include "../../../inc/scheduler/schedulable_groups/parallel_group.h"

//Pretty sure this function is completely useless
//ParallelGroup::ParallelGroup(std::vector<Schedulable> functionList, unsigned char requirements)
//	: GroupBase(GetRequirementFlag(functionList) | requirements, SchedulerTypes::Parallel)
//{
//	
//}

ParallelGroup::ParallelGroup(std::vector<std::shared_ptr<Schedulable>> schedulablesToSchedule)
	: ReschedulableGroup(GetRequirementFlags(schedulablesToSchedule), GroupTypes::Parallel)
{
	for (std::shared_ptr<Schedulable> schedulable : schedulablesToSchedule)
	{
		GroupBase::Schedule(schedulable);
	}
}