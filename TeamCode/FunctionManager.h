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
		std::function<bool()> backingFunction; //Change to virtual function

		virtual bool Run();

	protected:
		Scheduleable(unsigned char requirementFlags);

	public:
		//virtual void ThisIsAbstract() = 0;

		bool IsDead;
		unsigned char requirementFlags;

		Scheduleable(std::function<bool()> backingFunction, unsigned char requirementFlags);

		Scheduleable(std::function<bool()> backingFunction, Systems requiredSystem);

		//Scheduleable(std::function<bool()> backingFunctionCopy, unsigned char requirementFlags, bool passingByCopy);

		//Scheduleable(std::function<bool()> backingFunctionCopy, Systems requiredSystem, bool passingByCopy);

		//Scheduleable();

		Scheduleable(const Scheduleable& copyScheduelable);

		~Scheduleable();

		unsigned char GetRequirementFlags();

		void AddRequirement(unsigned char newRequirementFlags);

		bool RunIfReady(unsigned char availableSystem);

		void ResetAvailability();
	};

private:
	std::unordered_map<unsigned int, std::shared_ptr<Scheduleable>> database;
	//std::shared_ptr<Scheduleable>* database;
	unsigned int nextAvailableID;

public:

	FunctionManager();

	FunctionManager(const FunctionManager& copyFunctionManager);

	~FunctionManager();

	unsigned int AddToDatabase(std::shared_ptr<Scheduleable> scheduledItem);

	bool RunIfReady(unsigned int scheduledID, unsigned char availableSystem); //Runs the IScheduleable with the specified ID if the system requirements are met

	void Remove(unsigned int ID);

	void ResetAvailability(unsigned int ID);
};