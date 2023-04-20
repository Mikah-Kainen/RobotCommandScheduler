#pragma once
#include "Command.cpp"
#include <deque>
#include <vector>
#include <list>
#include <iostream>
#include "FunctionManager.h"
//#include "Manager.cpp"

static class Scheduler : public FunctionManager::Scheduleable
{
private:
	std::unordered_map<Systems, std::list<int>> schedule;
	FunctionManager functionManager;
	char currentlyRunningSystems;

public:
	std::vector<Systems> SchedulerSystems;

	Scheduler(std::vector<Systems> schedulerSystems);

	Scheduler(std::vector<char> schedulerSystems);

	Scheduler(char systemFlags);

	void Schedule(std::function<bool()> function, char requirementFlags);

	void Update();

	bool Run();
};

