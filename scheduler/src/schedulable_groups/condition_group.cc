#include "schedulable_groups/condition_group.h"

ConditionWrapper::ConditionWrapper(std::shared_ptr<Schedulable> schedulableToWrap, std::function<bool()> condition)
	:ReschedulableGroup(schedulableToWrap->GetRequirementFlags(), GroupTypes::Conditional), condition{ condition } 
{
	int schedulablePackedID = GroupBase::Schedule(schedulableToWrap);
	//initializeFunctions.push_back([&, currentID](GroupBase& group) {group.SetBar(currentID); });
	//do something like this
}

ConditionWrapper::ConditionWrapper(const ConditionWrapper& copyConditionWrapper)
	:ReschedulableGroup(copyConditionWrapper), condition{ copyConditionWrapper.condition } {}

void ConditionWrapper::Initialize()
{
	SetGroupEndEarly(!condition());
	ReschedulableGroup::Initialize();
}