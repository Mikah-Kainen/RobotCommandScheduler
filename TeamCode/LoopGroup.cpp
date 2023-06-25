//#include "../../inc/SchedulerInc/LoopGroup.h"
#include "LoopGroup.h"

bool LoopGroup::Return(bool isFinished)
{
	if (isFinished)
	{
		if (endCondition(*this))
		{
#ifndef DebugMode
			currentIteration = 0;
#endif

#ifdef DebugMode
			shouldResetIterations = true;
#endif
			return true;
		}
		else
		{
			InitializeBody();
			currentIteration++;
			return false;
		}
	}
	return false;
}

#ifdef DebugMode
void LoopGroup::InitializeBody()
{
	if (shouldResetIterations)
	{
		currentIteration = 0;
		shouldResetIterations = false;
	}
	RescheduleableGroup::InitializeBody();
}
#endif

LoopGroup::LoopGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule, std::function<bool(LoopGroup&)> endCondition)
	:RescheduleableGroup(GetRequirementFlags(scheduleablesToSchedule), SchedulerTypes::Loop), endCondition{ endCondition }, currentIteration{ 0 }
{
	//initializeFunctions.push_back([&](GroupBase& group) {((LoopGroup&)group).currentIteration = 0; });
#ifdef DebugMode
	shouldResetIterations = true;
#endif
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
#ifdef DebugMode
	shouldResetIterations = copyLoopGroup.shouldResetIterations;
#endif
}