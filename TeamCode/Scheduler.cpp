#include "Scheduler.h"


//Scheduler::ScheduledFunction::ScheduledFunction(std::function<bool()>& backingFunction, unsigned char requirementFlags)
//	:backingFunction{ backingFunction }, RequirementFlags{ requirementFlags } {}

//bool Scheduler::ScheduledFunction::Run()
//{
//	return backingFunction();
//}


Scheduler::Scheduler()
	: Scheduler(GetSystems(255), SchedulerTypes::Base)
{
}

Scheduler::Scheduler(std::vector<Systems> schedulerSystems, SchedulerTypes type)
	:Scheduleable(std::function<bool()>([&]() {return Run(); })), SchedulerSystems{schedulerSystems}, functionManager {FunctionManager()}
{	
	schedulerType = type;

	schedule = std::unordered_map<Systems, std::list<int>>();
	for(Systems system : schedulerSystems)
	{
		schedule.emplace(system, std::list<int>());
	}
}

Scheduler::Scheduler(unsigned char systemFlags, SchedulerTypes type)
	:Scheduler(GetSystems(systemFlags), type)
{
}

Scheduler::Scheduler(std::vector<unsigned char> systemFlags, SchedulerTypes type)
	:Scheduler(GetSystems(systemFlags), type)
{
}


//make it so commands can be scheduled with their parameters passed in(and the FunctionManager properly owns everything)

//add additional scheduling functions to handle scheduling Schedulers
//also think about how conditional functions could work(maybe a placeholder function that is replaced by a Scheduler that handles whichever branch it is currently on)
//also add functionality to remove systems from the scheduler when all the system's commands have been completed, so that the parent scheduler can start scheduling things with that system(but don't remove systems from the original scheduler!)
//make different types of schedulers or have information in the schedulers which decides whether a system should be removed or its default functions should be run when the scheduler runs out of scheduled functions for that system
void Scheduler::Schedule(std::function<bool()> function, unsigned char requirementFlags)
{
#pragma region workingTheory
	/*
	* char needs to be unsigned
	* use masks instead of shifting
	void SchedulerTest(unsigned char requirementFlags)
{
	for (int i = 0; i < 8; i++)
	{
		if (i == 7)
		{
			unsigned char temp = requirementFlags;
			requirementFlags = 'a';
			requirementFlags = temp;
		}
		//unsigned char currentFlag = requirementFlags << (8 - 1 - i);
		//currentFlag = currentFlag >> (8 - 1 - i);
		unsigned char currentMask = 1 << i;
		if ((requirementFlags & currentMask) >> i == 1)
		{
			std::cout << (pow(2, i)) << std::endl;
			//schedule[(Systems)pow(2, i)].push_back(newID); //MAKE SURE TO TEST THIS
			//int //Test ^
		}
	}
	//Definitely put stuff here
}*/
#pragma endregion
	int newID = functionManager.AddToDatabase(FunctionManager::Scheduleable(function, requirementFlags));
	for (int i = 0; i < SystemsCount; i++)
	{
		unsigned char currentFlag = requirementFlags << (i - 1);
		currentFlag = currentFlag >> (i - 1);
		if (currentFlag == 1)
		{

			schedule[(Systems)pow(2, i)].push_back(newID); //MAKE SURE TO TEST THIS
			int //Test ^
		}
	}
	float requirementNumber = (float)requirementFlags;
	int functionIndex = log(requirementNumber) / log(2);
	//Definitely put stuff here
}

void Scheduler::Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems)
{
	Schedule(function, CreateFlag(requiredSystems));
}

Scheduler& Scheduler::GetInstance()
{
	static Scheduler scheduler = Scheduler();
	return scheduler;
}

void Scheduler::Update()
{
	//This is probably unnecessary
}

bool Scheduler::Run()
{
	unsigned char availableSystem = (unsigned char)0x1;
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