#pragma once
#include "RescheduleableGroup.h"

class LoopGroup : public RescheduleableGroup
{
private:
	std::function<bool(LoopGroup&)> endCondition;
	bool Return(bool isFinished) override;

public:
	int currentIteration;

	LoopGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule, std::function<bool(LoopGroup&)> endCondition);

	LoopGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule, int loopCount);

	LoopGroup(const LoopGroup& copyLoopGroup);
};