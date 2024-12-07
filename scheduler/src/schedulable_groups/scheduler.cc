#include "../../include/schedulable_groups/scheduler.h"

Scheduler::Scheduler()
	: ConsumingGroup((unsigned char)Systems::All, GroupTypes::Base)
{
}


Scheduler& Scheduler::GetInstance()
{
	static Scheduler scheduler = Scheduler();
	return scheduler;
}

void Scheduler::Schedule(std::shared_ptr<Schedulable> schedulable)
{
	unsigned int ID = GroupBase::Schedule(schedulable);
	//AddToInitialize([&, ID](GroupBase& group) {group.SubscribeToEnd(ID, [&]() {functionMan})});
}

void Scheduler::Schedule(std::function<bool()> function, unsigned char requirementFlags)
{
	unsigned int ID = GroupBase::Schedule(function, requirementFlags);
}

void Scheduler::Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems)
{
	unsigned int ID = GroupBase::Schedule(function, requiredSystems);
}
