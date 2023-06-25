#pragma once
#include "RescheduleableGroup.h"

#define DebugMode

class LoopGroup : public RescheduleableGroup
{
private:
	std::function<bool(LoopGroup&)> endCondition;
#ifdef DebugMode
	bool shouldResetIterations;
#endif

	bool Return(bool isFinished) override;

#ifdef DebugMode
	void InitializeBody() override;
#endif

public:
	int currentIteration;

	LoopGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule, std::function<bool(LoopGroup&)> endCondition);

	LoopGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule, int loopCount);

	LoopGroup(const LoopGroup& copyLoopGroup);
};