#pragma once
#include <functional>
#include <unordered_map>
#include <memory>
#include <exception>

//Interesting link: https://jguegant.github.io/blogs/tech/performing-try-emplace.html
class FunctionManager
{
public:
	class IScheduleable
	{
	private:
		char availableSystems;
		char requirementFlags;
		std::function<bool()> backingFunction;

	public:
		bool IsDead;
		IScheduleable(std::function<bool()> backingFunction, char requirementFlags);

		IScheduleable(std::function<bool()> backingFunction);

		IScheduleable();

		~IScheduleable();

		void AddRequirement(char newRequirementFlags);

		bool RunIfReady(char availableSystem);
	};

private:
	std::unordered_map<int, IScheduleable> database;
	int nextAvailableID;

public:

	FunctionManager();

	int AddToDatabase(IScheduleable scheduledItem);

	bool RunIfReady(int scheduledID, char availableSystem); //Runs the IScheduleable with the specified ID if the system requirements are met

	void Remove(int ID);
};

