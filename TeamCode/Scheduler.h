#pragma once
#include "Command.cpp"
#include <deque>
#include <vector>
#include <list>
#include <iostream>
#include "FunctionManager.h"
//#include "Manager.cpp"

static class Scheduler
{
private:
	//class ScheduledFunction
	//{
	//public:
	//	char AvailableSystems;
	//	char RequirementFlags;

	//	ScheduledFunction(std::function<bool()>& backingFunction, char requirementFlags);

	//	bool Run();

	//private:
	//	std::function<bool()> backingFunction;
	//};

	//Manager<ScheduledFunction> functions;
	std::list<int>* schedule;
	FunctionManager functionManager;
	char currentlyRunningSystems;

	Scheduler();

public:
	int SystemsCount;

	static Scheduler& GetInstance();

	void Schedule(std::function<bool()> function, char requirementFlags);

	void Update();

	void Run();
};

