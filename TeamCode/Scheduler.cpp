#include "Scheduler.h"


//Scheduler::ScheduledFunction::ScheduledFunction(std::function<bool()>& backingFunction, char requirementFlags)
//	:backingFunction{ backingFunction }, RequirementFlags{ requirementFlags } {}

//bool Scheduler::ScheduledFunction::Run()
//{
//	return backingFunction();
//}


Scheduler::Scheduler()
	:IScheduleable(std::function<bool()>([&]() {return Run(); })), SystemsCount{ (int)Systems::Other + 1 }, functionManager{ FunctionManager() }
{	
	schedule = new std::list<int>[SystemsCount];
	for (int i = 0; i < SystemsCount; i ++)
	{
		std::list<int> newList = std::list<int>();
		schedule[i] = newList;
	}
}

Scheduler& Scheduler::GetInstance()
{
	static Scheduler scheduler;
	return scheduler;
}

void Scheduler::Schedule(std::function<bool()> function, char requirementFlags)
{
	int newID = functionManager.AddToDatabase(FunctionManager::IScheduleable(function, requirementFlags));
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