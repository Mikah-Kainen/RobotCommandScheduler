#include "LoopGroup.h"

//#define DebugMode

bool LoopGroup::Return(bool isFinished)
{
	if (isFinished)
	{
		if (endCondition())
		{
#ifndef DebugMode
			currentIteration = 0;
#endif

#ifdef DebugMode
			shouldResetIterations = true; //technically I can just 
#endif
		}
	}
}

#ifdef DebugMode
void LoopGroup::Initialize()
{
	if (shouldResetIterations)
	{
		currentIteration = 0;
		shouldResetIterations = false;
	}
	RescheduleableGroup::Initialize();
}
#endif

LoopGroup::LoopGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule, std::function<bool()> endCondition)
	:RescheduleableGroup(GetRequirementFlags(scheduleablesToSchedule), SchedulerTypes::Loop), endCondition{ endCondition }, currentIteration{ 0 }, shouldResetIterations{true}
{
	//initializeFunctions.push_back([&](GroupBase& group) {((LoopGroup&)group).currentIteration = 0; });
}

LoopGroup::LoopGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule, int loopCount)
	:LoopGroup(scheduleablesToSchedule, [&, loopCount]() {return currentIteration < loopCount; }) {}