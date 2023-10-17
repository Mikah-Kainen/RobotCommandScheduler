#pragma once
#include "RescheduleableGroup.h"



class ConditionScheduleable : public RescheduleableGroup
{
private:
	std::function<bool()> condition;

protected:

public:
	ConditionScheduleable(std::shared_ptr<Scheduleable> scheduleableToWrap, std::function<bool()> condition);

	ConditionScheduleable(const ConditionScheduleable& copyConditionalGroup);

	void Initialize() override;

	static std::shared_ptr<ConditionScheduleable> RunConditionally(std::shared_ptr<Scheduleable> scheduleable, std::function<bool()> condition);
};

