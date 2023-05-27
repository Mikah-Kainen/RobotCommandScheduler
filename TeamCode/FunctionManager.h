#pragma once
#include <functional>
#include <unordered_map>
#include <memory>
#include <exception>
#include "Static.h"


//Interesting link: https://jguegant.github.io/blogs/tech/performing-try-emplace.html
class FunctionManager
{
public:
	class Scheduleable
	{
	private:
		unsigned char availableSystems;
		const std::function<bool()>& backingFunction;

	//protected:
	public:
		//virtual void ThisIsAbstract() = 0;

		unsigned char requirementFlags;
		Scheduleable(const std::function<bool()>& backingFunction, unsigned char requirementFlags);

		Scheduleable(const std::function<bool()>& backingFunction, Systems requiredSystem);

		//Scheduleable();

		bool IsDead;

		~Scheduleable();

		unsigned char GetRequirementFlags();

		void AddRequirement(unsigned char newRequirementFlags);

		bool RunIfReady(unsigned char availableSystem);

		void ResetAvailability();
	};

private:
	std::unordered_map<int, std::shared_ptr<Scheduleable>> database;
	int nextAvailableID;

public:

	FunctionManager();

	int AddToDatabase(std::shared_ptr<Scheduleable> scheduledItem);

	bool RunIfReady(int scheduledID, unsigned char availableSystem); //Runs the IScheduleable with the specified ID if the system requirements are met

	void Remove(int ID);

	void ResetAvailability(int ID);
};