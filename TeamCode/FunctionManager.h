#pragma once
#include <functional>
#include <unordered_map>
#include <memory>
#include <exception>

class FunctionManager
{
public:
	class IScheduleable;

private:
	std::unordered_map<int, std::unique_ptr<IScheduleable>> database;
	int nextAvailableID;

public:
	class IScheduleable
	{
	private:
		char availableSystems;
		char requirementFlags;
		std::function<bool()> backingFunction;

	public:
		IScheduleable(std::function<bool()> backingFunction, char requirementFlags);

		IScheduleable(std::function<bool()> backingFunction);

		void AddRequirement(char newRequirementFlags);

		bool RunIfReady(char availableSystem);
	};

	FunctionManager();

	int AddToDatabase(IScheduleable& scheduledItem);

	bool RunIfReady(int scheduledID, char availableSystem);
};

