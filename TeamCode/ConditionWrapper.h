#pragma once
#include "RescheduleableGroup.h"



class ConditionWrapper : public RescheduleableGroup
{
private:
	std::function<bool()> condition;

protected:

public:
	ConditionWrapper(std::shared_ptr<Scheduleable> scheduleableToWrap, std::function<bool()> condition);

	ConditionWrapper(const ConditionWrapper& copyConditionWrapper);

	void Initialize() override;
};

