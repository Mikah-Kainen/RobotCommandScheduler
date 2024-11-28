#include "schedulable_groups/loop_group.h"

bool LoopGroup::Return(bool isFinished)
{
	if (isFinished)
	{
		currentIteration++;
		Initialize();
		if (endCondition(*this))
		{
			currentIteration = 0;
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

LoopGroup::LoopGroup(std::vector<std::shared_ptr<Schedulable>> schedulablesToSchedule, std::function<bool(LoopGroup&)> endCondition)
	:ReschedulableGroup(GetRequirementFlags(schedulablesToSchedule), GroupTypes::Loop), endCondition{ endCondition }, currentIteration{ 0 }
{
	//initializeFunctions.push_back([&](GroupBase& group) {((LoopGroup&)group).currentIteration = 0; });
	for (std::shared_ptr<Schedulable> schedulable : schedulablesToSchedule)
	{
		GroupBase::Schedule(schedulable);
	}
}

LoopGroup::LoopGroup(std::vector<std::shared_ptr<Schedulable>> schedulablesToSchedule, int loopCount)
	:LoopGroup(schedulablesToSchedule, [&, loopCount](LoopGroup& loopGroup) {return loopGroup.currentIteration >= loopCount; }) {}

LoopGroup::LoopGroup(const LoopGroup& copyLoopGroup)
	:ReschedulableGroup(copyLoopGroup), endCondition{ copyLoopGroup.endCondition }, currentIteration{ copyLoopGroup.currentIteration }
{
}