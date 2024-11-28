#pragma once
#include "base_groups/reschedulable_group.h"

class LoopGroup : public ReschedulableGroup
{
private:
	std::function<bool(LoopGroup&)> endCondition;
	bool Return(bool isFinished) override;

public:
	int currentIteration;

	LoopGroup(std::vector<std::shared_ptr<Schedulable>> schedulablesToSchedule, std::function<bool(LoopGroup&)> endCondition);

	LoopGroup(std::vector<std::shared_ptr<Schedulable>> schedulablesToSchedule, int loopCount);

	LoopGroup(const LoopGroup& copyLoopGroup);
};