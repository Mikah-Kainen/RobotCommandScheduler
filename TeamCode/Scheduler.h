#pragma once
#include "Command.cpp"
#include <deque>
#include <vector>
#include <memory>

class ScheduledFunction
{
public:
	char AvailableSystems;
	char RequirementFlags;

	ScheduledFunction(std::function<bool()> backingFunction, char requirementFlags);

	bool Run();

private:
	std::function<bool()> backingFunction;
};

class Scheduler
{

private:
	std::deque<std::unique_ptr<ScheduledFunction>>* requirementDeques; //probably change these to linked lists at some point
	std::vector<std::unique_ptr<ScheduledFunction>> runningFunctions;
	char currentlyRunningSystems;

public:
	int SystemsCount;

	Scheduler();

	void Update();

	void Run();
};

