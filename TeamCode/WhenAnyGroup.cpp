//#include "../../inc/Scheduler/WhenAnyGroup.h"
#include "WhenAnyGroup.h"

WhenAnyGroup::WhenAnyGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule)
	:RescheduleableGroup(GetRequirementFlags(scheduleablesToSchedule), GroupTypes::Sequential)
{
	for (int i = 0; i < scheduleablesToSchedule.size(); i++)
	{
		int currentID = GroupBase::Schedule(scheduleablesToSchedule[i]);
		SubscribeToEnd(currentID, std::function<void(GroupBase&)>([&](GroupBase& group) {group.SetGroupEndEarly(true); }));
	}
}