//#include "../../inc/Scheduler/ConditionWrapper.h"
#include "ConditionWrapper.h"

ConditionWrapper::ConditionWrapper(std::shared_ptr<Scheduleable> scheduleableToWrap, std::function<bool()> condition)
	:RescheduleableGroup(scheduleableToWrap->GetRequirementFlags(), GroupTypes::Conditional), condition{ condition } 
{
	int scheduleablePackedID = GroupBase::Schedule(scheduleableToWrap);
	//initializeFunctions.push_back([&, currentID](GroupBase& group) {group.SetBar(currentID); });
	//do something like this
}

ConditionWrapper::ConditionWrapper(const ConditionWrapper& copyConditionWrapper)
	:RescheduleableGroup(copyConditionWrapper), condition{ copyConditionWrapper.condition } {}

void ConditionWrapper::Initialize()
{
	SetGroupEndEarly(!condition());
	RescheduleableGroup::Initialize();
}