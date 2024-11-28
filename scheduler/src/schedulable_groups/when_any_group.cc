#include "schedulable_groups/when_any_group.h"

WhenAnyGroup::WhenAnyGroup(std::vector<std::shared_ptr<Schedulable>> schedulablesToSchedule)
	:ReschedulableGroup(GetRequirementFlags(schedulablesToSchedule), GroupTypes::Sequential)
{
	for (int i = 0; i < schedulablesToSchedule.size(); i++)
	{
		int currentID = GroupBase::Schedule(schedulablesToSchedule[i]);
		SubscribeToEnd(currentID, std::function<void(GroupBase&)>([&](GroupBase& group) {group.SetGroupEndEarly(true); }));
	}
}