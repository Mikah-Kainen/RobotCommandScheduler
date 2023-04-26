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