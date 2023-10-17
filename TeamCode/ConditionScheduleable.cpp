#include "ConditionScheduleable.h"

ConditionScheduleable::ConditionScheduleable(std::shared_ptr<Scheduleable> scheduleableToWrap, std::function<bool()> condition)
	:RescheduleableGroup(scheduleableToWrap->GetRequirementFlags(), GroupTypes::Conditional), condition{ condition } 
{
	int scheduleablePackedID = GroupBase::Schedule(scheduleableToWrap);
	//initializeFunctions.push_back([&, currentID](GroupBase& group) {group.SetBar(currentID); });
	//do something like this
}

ConditionScheduleable::ConditionScheduleable(const ConditionScheduleable& copyConditionalGroup)
	:RescheduleableGroup(copyConditionalGroup), condition{ copyConditionalGroup.condition } {}

void ConditionScheduleable::Initialize()
{
	SetGroupEndEarly(!condition());
}

std::shared_ptr<ConditionScheduleable> ConditionScheduleable::RunConditionally(std::shared_ptr<Scheduleable> scheduleable, std::function<bool()> condition)
{
	return std::make_shared<ConditionScheduleable>(ConditionScheduleable(scheduleable, condition));
}