#include "Scheduler.h"


//Scheduler::ScheduledFunction::ScheduledFunction(std::function<bool()>& backingFunction, char requirementFlags)
//	:backingFunction{ backingFunction }, RequirementFlags{ requirementFlags } {}

//bool Scheduler::ScheduledFunction::Run()
//{
//	return backingFunction();
//}


//make it so that Schedulers take in a char for their requirement flags
//make it so commands can be scheduled with their parameters passed in(and the FunctionManager properly owns everything)

Scheduler::Scheduler(std::vector<Systems> schedulerSystems)
	:Scheduleable(std::function<bool()>([&]() {return Run(); })), SchedulerSystems{schedulerSystems}, functionManager {
	FunctionManager()
}
{	
	schedule = std::unordered_map<Systems, std::list<int>>();
	for(Systems system : schedulerSystems)
	{
		schedule.emplace(system, std::list<int>());
	}
}

//add additional scheduler constructors
Scheduler::Scheduler()
{

}

//add additional scheduling functions to handle scheduling Schedulers
//also think about how conditional functions could work(maybe a placeholder function that is replaced by a Scheduler that handles whichever branch it is currently on)
//also add functionality to remove systems from the scheduler when all the system's commands have been completed, so that the parent scheduler can start scheduling things with that system(but don't remove systems from the original scheduler!)
//make different types of schedulers or have information in the schedulers which decides whether a system should be removed or its default functions should be run when the scheduler runs out of scheduled functions for that system
void Scheduler::Schedule(std::function<bool()> function, char requirementFlags)
{
	int newID = functionManager.AddToDatabase(FunctionManager::Scheduleable(function, requirementFlags));
	float requirementNumber = (float)requirementFlags;
	int functionIndex = log(requirementNumber) / log(2);
	schedule[functionIndex].push_back(newID);
	//Definitely put stuff here
}

void Scheduler::Update()
{
	//This is probably unnecessary
}

bool Scheduler::Run()
{
	char availableSystem = (char)0x1;
	for (int i = 0; i < SystemsCount; i ++)
	{
		std::list<int>& currentSystemSchedule = schedule[i];
		if (currentSystemSchedule.size() == 0)
		{
			//std::cout << "No functions are schedule for System-" << i << "\n";
		}
		else if (functionManager.RunIfReady(currentSystemSchedule.front(), availableSystem))
		{
			functionManager.Remove(currentSystemSchedule.front());
			currentSystemSchedule.pop_front();
		}
		availableSystem <<= 1;
		//std::unique_ptr<ScheduledFunction>& currentFunction = functions.Get(currentSystemSchedule.front());
		//if (currentFunction->RequirementFlags & currentlyRunningSystems == 0)
		//{
		//	currentFunction->AvailableSystems |= (1 << i);
		//	if (currentFunction->AvailableSystems == currentFunction->RequirementFlags)
		//	{
		//		currentlyRunningSystems |= currentFunction->RequirementFlags;
		//		if (currentFunction->Run())
		//		{
		//			currentSystemSchedule.pop_front();
		//		}
		//		//think about if I should ever reset the AvailableSystems of a function(maybe in Update?)
		//		//(i.e. does a system which was once available for ever become unavailable for that function before that function finishes?)
		//	}
		//}
	}

	return false; //Change this!!
}