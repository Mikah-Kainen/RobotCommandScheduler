#pragma once
#include "../base_groups/reschedulable_group.h"



class ConditionWrapper : public ReschedulableGroup
{
private:
	std::function<bool()> condition;

protected:

public:
	ConditionWrapper(std::shared_ptr<Schedulable> schedulableToWrap, std::function<bool()> condition);

	ConditionWrapper(const ConditionWrapper& copyConditionWrapper);

	void Initialize() override;
};

