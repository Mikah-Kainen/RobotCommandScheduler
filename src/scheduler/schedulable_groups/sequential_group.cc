#include "../../../inc/scheduler/schedulable_groups/sequential_group.h"

SequentialGroup::SequentialGroup(std::vector<std::shared_ptr<Schedulable>> schedulablesToSchedule)
	:ReschedulableGroup(GetRequirementFlags(schedulablesToSchedule), GroupTypes::Sequential)
{
	unsigned int previousID = GroupBase::Schedule(schedulablesToSchedule[0]);
	unsigned int currentID;
	for (int i = 1; i < schedulablesToSchedule.size(); i ++)
	{
		currentID = GroupBase::Schedule(schedulablesToSchedule[i]);
		initializeFunctions.push_back([&, currentID](GroupBase& group) {group.AddBar(currentID); });
		SubscribeToEnd(previousID, std::function<void(GroupBase&)>([&, currentID](GroupBase& group) {group.RemoveBar(currentID); }));
		//GroupBase::SetBar(currentID);
		//GroupBase::SubscribeToEnd(previousID, std::function<void()>([&, currentID]() {group.RefmoveBar(currentID);}));
		previousID = currentID;
	}
}