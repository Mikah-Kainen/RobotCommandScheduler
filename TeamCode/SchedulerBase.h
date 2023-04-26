#pragma once
#include "Command.cpp"
#include <deque>
#include <vector>
#include <list>
#include <iostream>
#include "FunctionManager.h"
#include "Static.h"
//#include "Manager.cpp"

//int
//Schedulers should not be made in the main project; rather, a static scheduler(the only Base) can be part of another class and shared with the user via that
//Have a different scheduler class for each SchedulerType. The base is what I have now. BaseScheduler is a singleton and can just be called Scheduler. Parallel and Sequential Schedulers take in a list of Scheduleables and create a scheduler.
//P.S. Make Scheduleable a unique_pointer instead of a raw pointer in FunctionManager
class SchedulerBase : public FunctionManager::Scheduleable
{
protected:
	static enum class SchedulerTypes
	{
		Base,
		Parallel,
		Sequential,
	};

private:
	std::unordered_map<Systems, std::list<int>> schedule;
	FunctionManager functionManager;
	SchedulerTypes schedulerType;
	unsigned char currentlyRunningSystems;

protected:
	SchedulerBase(unsigned char systemFlags, SchedulerTypes type);

	SchedulerBase(std::vector<unsigned char> systemFlags, SchedulerTypes type);

	SchedulerBase(std::vector<Systems> schedulerSystems, SchedulerTypes type);

public:
	//Test GetSystems in Static because it is a pretty useful loop

	void Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems);

	void Schedule(std::function<bool()> function, unsigned char requirementFlags);


	void Update();

	bool Run();
};

