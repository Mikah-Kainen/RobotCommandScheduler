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
//Change name from GroupBase to GroupBase
class GroupBase : public FunctionManager::Scheduleable
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
	bool shouldStoreScheduleables;
	std::vector<std::shared_ptr<FunctionManager::Scheduleable>> storedScheduleables; //stores scheduleables so they aren't deleted after running(not necessary for main Scheduler)
	
	std::unordered_map<Systems, std::list<int>> schedule;
	FunctionManager functionManager;
	SchedulerTypes schedulerType;
	unsigned char currentlyRunningSystems;
	int schedulerID;


	GroupBase(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleablesToStore, bool shouldStoreScheduleables, unsigned char systemFlags, SchedulerTypes type);

protected:

	GroupBase(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleablesToStore, unsigned char systemFlags, SchedulerTypes type);

	GroupBase(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleablesToStore, std::vector<unsigned char> systemFlags, SchedulerTypes type);

	GroupBase(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleablesToStore, std::vector<Systems> schedulerSystems, SchedulerTypes type);


	GroupBase(unsigned char systemFlags, SchedulerTypes type);

	GroupBase(std::vector<unsigned char> systemFlags, SchedulerTypes type);

	GroupBase(std::vector<Systems> schedulerSystems, SchedulerTypes type);


	void Schedule(std::shared_ptr<Scheduleable> scheduleable);

	void Schedule(std::function<bool()> function, unsigned char requirementFlags);

	void Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems);

public:
	GroupBase(const GroupBase& copyGroupBase);
	
	void Update();

	bool Run() override;
};

