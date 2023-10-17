#pragma once
#include "RescheduleableGroup.h"



class ConditionWrapper : public RescheduleableGroup
{
private:
	std::function<bool()> condition;

protected:

public:
	ConditionWrapper(std::shared_ptr<Scheduleable> scheduleableToWrap, std::function<bool()> condition);

	ConditionWrapper(const ConditionWrapper& copyConditionalGroup);

	void Initialize() override;

	static std::shared_ptr<ConditionWrapper> RunConditionally(std::shared_ptr<Scheduleable> scheduleable, std::function<bool()> condition);
};

