#pragma once
#include "Command.cpp"
#include <deque>
#include <vector>
#include <memory>

static class Scheduler
{
private:
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

	std::deque<std::unique_ptr<ScheduledFunction>>* requirementDeques; //probably change these to linked lists at some point
	std::vector<std::unique_ptr<ScheduledFunction>> runningFunctions;
	char currentlyRunningSystems;

	Scheduler();

public:
	int SystemsCount;

	static Scheduler& GetInstance();

	void Schedule(std::function<bool()> function, char requirementFlags);

	void Update();

	void Run();
};

