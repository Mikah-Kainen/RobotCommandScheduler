//#include "../../inc/SchedulerInc/LoopGroup.h"
#include "LoopGroup.h"

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

LoopGroup::LoopGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule, std::function<bool(LoopGroup&)> endCondition)
	:RescheduleableGroup(GetRequirementFlags(scheduleablesToSchedule), GroupTypes::Loop), endCondition{ endCondition }, currentIteration{ 0 }
{
	//initializeFunctions.push_back([&](GroupBase& group) {((LoopGroup&)group).currentIteration = 0; });
	for (std::shared_ptr<Scheduleable> scheduleable : scheduleablesToSchedule)
	{
		GroupBase::Schedule(scheduleable);
	}
}

LoopGroup::LoopGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule, int loopCount)
	:LoopGroup(scheduleablesToSchedule, [&, loopCount](LoopGroup& loopGroup) {return loopGroup.currentIteration >= loopCount; }) {}

LoopGroup::LoopGroup(const LoopGroup& copyLoopGroup)
	:RescheduleableGroup(copyLoopGroup), endCondition{ copyLoopGroup.endCondition }, currentIteration{ copyLoopGroup.currentIteration }
{
}