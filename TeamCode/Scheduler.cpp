#include "Scheduler.h"(


Scheduler::ScheduledFunction::ScheduledFunction(std::function<bool()>& backingFunction, char requirementFlags)
	:backingFunction{ backingFunction }, RequirementFlags{ requirementFlags } {}

bool Scheduler::ScheduledFunction::Run()
{
	return backingFunction();
}


Scheduler::Scheduler()
	:SystemsCount{ (int)Systems::Other }, schedule{ new std::list<int>[SystemsCount] }, functions{Manager<ScheduledFunction>()}
{	
	for (int i = 0; i < SystemsCount; i ++)
	{
		schedule[i] = std::list<int>();
	}
}

Scheduler& Scheduler::GetInstance()
{
	static Scheduler scheduler;
	return scheduler;
}

void Scheduler::Schedule(std::function<bool()>& function, char requirementFlags)
{

}

void Scheduler::Update()
{

}

void Scheduler::Run()
{
	for (int i = 0; i < SystemsCount; i ++)
	{
		std::list<int>& currentSystemSchedule = schedule[i];
		std::unique_ptr<ScheduledFunction>& currentFunction = functions.Get(currentSystemSchedule.front());
		if (currentFunction->RequirementFlags & currentlyRunningSystems == 0)
		{
			currentFunction->AvailableSystems |= (1 << i);
			if (currentFunction->AvailableSystems == currentFunction->RequirementFlags)
			{
				currentlyRunningSystems |= currentFunction->RequirementFlags;
				if (currentFunction->Run())
				{
					currentSystemSchedule.pop_front();
				}
				//think about if I should ever reset the AvailableSystems of a function(maybe in Update?)
				//(i.e. does a system which was once available for ever become unavailable for that function before that function finishes?)
			}
		}
	}
}