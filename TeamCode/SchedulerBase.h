#pragma once
#include <deque>
#include <vector>
#include <list>
#include <iostream>
#include "FunctionManager.h"
#include "Static.h"

//int
//Schedulers should not be made in the main project; rather, a static scheduler(the only Base) can be part of another class and shared with the user via that
//Have a different scheduler class for each SchedulerType. The base is what I have now. BaseScheduler is a singleton and can just be called Scheduler. Parallel and Sequential Schedulers take in a list of Scheduleables and create a scheduler.
//P.S. Make Scheduleable a unique_pointer instead of a raw pointer in FunctionManager
class SchedulerBase : public FunctionManager::Scheduleable
{
protected:
	static unsigned char GetRequirementFlags(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleables);

	enum class SchedulerTypes
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
	int schedulerID;

protected:
	SchedulerBase(unsigned char systemFlags, SchedulerTypes type);

	SchedulerBase(std::vector<unsigned char> systemFlags, SchedulerTypes type);

	SchedulerBase(std::vector<Systems> schedulerSystems, SchedulerTypes type);


	void Schedule(std::shared_ptr<Scheduleable> scheduleable);

	void Schedule(const std::function<bool()>& function, unsigned char requirementFlags);

	void Schedule(const std::function<bool()>& function, std::vector<Systems> requiredSystems);

public:
	SchedulerBase(const SchedulerBase& copySchedulerBase);
	
	void Update();

	bool Run();
};

