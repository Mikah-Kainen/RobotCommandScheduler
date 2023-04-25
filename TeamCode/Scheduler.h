#pragma once
#include "Command.cpp"
#include <deque>
#include <vector>
#include <list>
#include <iostream>
#include "FunctionManager.h"
#include "Static.h"
//#include "Manager.cpp"

static class Scheduler : public FunctionManager::Scheduleable
{
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
	
	Scheduler();


public:
	std::vector<Systems> SchedulerSystems; //this defeats the point of having flags. Add a unsigned char and when I loop, just make sure current index is zero. Also test GetSystems in Static because it is a pretty useful loop

	Scheduler(std::vector<Systems> schedulerSystems, SchedulerTypes type);

	Scheduler(unsigned char systemFlags, SchedulerTypes type);

	Scheduler(std::vector<unsigned char> systemFlags, SchedulerTypes type);


	void Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems);

	void Schedule(std::function<bool()> function, unsigned char requirementFlags);

	static Scheduler& GetInstance();

	void Update();

	bool Run();
};

