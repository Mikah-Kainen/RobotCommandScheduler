//#include "../../inc/SchedulerInc/Scheduler.h"
#include "Scheduler.h"

Scheduler::Scheduler()
	: GroupBase((unsigned char)Systems::All, SchedulerTypes::Base)
{
}


Scheduler& Scheduler::GetInstance()
{
	static Scheduler scheduler = Scheduler();
	return scheduler;
}

void Scheduler::Schedule(std::shared_ptr<Scheduleable> scheduleable)
{
	GroupBase::Schedule(scheduleable);
}

void Scheduler::Schedule(std::function<bool()> function, unsigned char requirementFlags)
{
	GroupBase::Schedule(function, requirementFlags);
}

void Scheduler::Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems)
{
	GroupBase::Schedule(function, requiredSystems);
}
