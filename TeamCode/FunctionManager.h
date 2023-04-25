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
		unsigned char availableSystems;
		unsigned char requirementFlags;
		std::function<bool()> backingFunction;

	public:
		bool IsDead;
		Scheduleable(std::function<bool()> backingFunction, unsigned char requirementFlags);

		Scheduleable(std::function<bool()> backingFunction);

		Scheduleable();

		~Scheduleable();

		void AddRequirement(unsigned char newRequirementFlags);

		bool RunIfReady(unsigned char availableSystem);
	};

private:
	std::unordered_map<int, Scheduleable> database;
	int nextAvailableID;

public:

	FunctionManager();

	int AddToDatabase(Scheduleable scheduledItem);

	bool RunIfReady(int scheduledID, unsigned char availableSystem); //Runs the IScheduleable with the specified ID if the system requirements are met

	void Remove(int ID);
};

