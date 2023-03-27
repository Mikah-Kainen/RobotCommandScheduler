#include "Scheduler.h"(


ScheduledFunction::ScheduledFunction(std::function<bool()> backingFunction, char requirementFlags)
	:backingFunction{ backingFunction }, RequirementFlags{requirementFlags} {}

bool ScheduledFunction::Run()
{
	return backingFunction();
}


Scheduler::Scheduler()
{	
	SystemsCount = (int)Systems::Other;
	requirementDeques = new std::deque<std::unique_ptr<ScheduledFunction>>[SystemsCount];
	for (int i = 0; i < SystemsCount; i ++)
	{
		requirementDeques[i] = std::deque<std::unique_ptr<ScheduledFunction>>();
	}
}

void Scheduler::Run()
{
	for (int i = 0; i < SystemsCount; i ++)
	{
		//using currentFunction = requirementDeques[i].front(); Ask Stan if there is a way to alias this
		if (requirementDeques[i].front()->RequirementFlags & currentlyRunningSystems == 0)
		{
			requirementDeques[i].front()->AvailableSystems |= (1 << i);
			if (requirementDeques[i].front()->AvailableSystems == requirementDeques[i].front()->RequirementFlags)
			{
				currentlyRunningSystems |= requirementDeques[i].front()->RequirementFlags;
				if (requirementDeques[i].front()->Run())
				{
					requirementDeques[i].pop_front();
				}
				//think about if I should ever reset the AvailableSystems of a function(maybe in Update?)
				//(i.e. does a system which was once available for ever become unavailable for that function before that function finishes?)
			}
		}
	}
}

void Scheduler::Update()
{

}