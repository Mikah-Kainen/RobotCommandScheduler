#pragma once
#include <functional>
#include <unordered_map>
#include <memory>
#include <exception>



//Interesting link: https://jguegant.github.io/blogs/tech/performing-try-emplace.html
class FunctionManager
{
public:
	class Scheduleable
	{
	private:
		char availableSystems;
		char requirementFlags;
		std::function<bool()> backingFunction;

	public:
		bool IsDead;
		Scheduleable(std::function<bool()> backingFunction, char requirementFlags);

		Scheduleable(std::function<bool()> backingFunction);

		Scheduleable();

		~Scheduleable();

		void AddRequirement(char newRequirementFlags);

		bool RunIfReady(char availableSystem);
	};

private:
	std::unordered_map<int, Scheduleable> database;
	int nextAvailableID;

public:

	FunctionManager();

	int AddToDatabase(Scheduleable scheduledItem);

	bool RunIfReady(int scheduledID, char availableSystem); //Runs the IScheduleable with the specified ID if the system requirements are met

	void Remove(int ID);
};

