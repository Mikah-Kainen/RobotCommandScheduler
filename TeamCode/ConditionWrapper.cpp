#include "ConditionWrapper.h"

ConditionWrapper::ConditionWrapper(std::shared_ptr<Scheduleable> scheduleableToWrap, std::function<bool()> condition)
	:RescheduleableGroup(scheduleableToWrap->GetRequirementFlags(), GroupTypes::Conditional), condition{ condition } 
{
	int scheduleablePackedID = GroupBase::Schedule(scheduleableToWrap);
	//initializeFunctions.push_back([&, currentID](GroupBase& group) {group.SetBar(currentID); });
	//do something like this
}

ConditionWrapper::ConditionWrapper(const ConditionWrapper& copyConditionalGroup)
	:RescheduleableGroup(copyConditionalGroup), condition{ copyConditionalGroup.condition } {}

void ConditionWrapper::Initialize()
{
	SetGroupEndEarly(!condition());
}

std::shared_ptr<ConditionWrapper> ConditionWrapper::RunConditionally(std::shared_ptr<Scheduleable> scheduleable, std::function<bool()> condition)
{
	return std::make_shared<ConditionWrapper>(ConditionWrapper(scheduleable, condition));
}