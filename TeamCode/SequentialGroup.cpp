//#include "../../inc/SchedulerInc/SequentialGroup.h"
#include "SequentialGroup.h"

SequentialGroup::SequentialGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule)
	:RescheduleableGroup(GetRequirementFlags(scheduleablesToSchedule), GroupTypes::Sequential)
{
	unsigned int previousID = GroupBase::Schedule(scheduleablesToSchedule[0]);
	unsigned int currentID;
	for (int i = 1; i < scheduleablesToSchedule.size(); i ++)
	{
		currentID = GroupBase::Schedule(scheduleablesToSchedule[i]);
		initializeFunctions.push_back([&, currentID](GroupBase& group) {group.AddBar(currentID); });
		SubscribeToEnd(previousID, std::function<void(GroupBase&)>([&, currentID](GroupBase& group) {group.RemoveBar(currentID); }));
		//GroupBase::SetBar(currentID);
		//GroupBase::SubscribeToEnd(previousID, std::function<void()>([&, currentID]() {group.RefmoveBar(currentID);}));
		previousID = currentID;
	}
}