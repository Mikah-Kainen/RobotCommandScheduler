#include "ConditionalGroup.h"

ConditionalGroup::ConditionalGroup(std::shared_ptr<Scheduleable> scheduleableToSchedule, std::function<bool()> condition)
	:RescheduleableGroup(scheduleableToSchedule->GetRequirementFlags(), SchedulerTypes::Conditional), condition{condition}
{

}

ConditionalGroup::ConditionalGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule, std::function<bool()> condition)
	:ConditionalGroup(std::make_shared<ParallelGroup>())
{
	for (std::shared_ptr<Scheduleable> scheduleable : scheduleablesToSchedule)
	{
		GroupBase::Schedule(scheduleable);
	}
}

ConditionalGroup::ConditionalGroup(const ConditionalGroup& copyConditionalGroup)
	:RescheduleableGroup(copyConditionalGroup), condition{ copyConditionalGroup.condition } {}

void ConditionalGroup::Initialize()
{

}