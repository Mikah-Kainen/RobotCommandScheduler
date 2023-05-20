//#include "../../inc/SchedulerInc/Scheduler.h"
#include "Scheduler.h"

Scheduler::Scheduler()
	: SchedulerBase((unsigned char)Systems::All, SchedulerTypes::Base)
{
}


Scheduler& Scheduler::GetInstance()
{
	static Scheduler scheduler = Scheduler();
	return scheduler;
}

void Scheduler::Schedule(Scheduleable* scheduleable)
{
	SchedulerBase::Schedule(scheduleable);
}

void Scheduler::Schedule(std::function<bool()> function, unsigned char requirementFlags)
{
	SchedulerBase::Schedule(function, requirementFlags);
}

void Scheduler::Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems)
{
	SchedulerBase::Schedule(function, requiredSystems);
}
