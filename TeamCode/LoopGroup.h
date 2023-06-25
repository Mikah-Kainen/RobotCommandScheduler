#pragma once
#include "RescheduleableGroup.h"

class LoopGroup : public RescheduleableGroup
{
private:
	std::function<bool()> endCondition;
	bool shouldResetIterations;

	bool Return(bool isFinished) override;

	void Initialize() override;

public:
	int currentIteration;

	LoopGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule, std::function<bool()> endCondition);

	LoopGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule, int loopCount);
};