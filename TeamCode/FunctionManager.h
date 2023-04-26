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
		unsigned char availableSystems;  //Right now this is never reset. Is there ever a case where a system that is available becomes unavailable? 
		//Yes, interrupting commands. Therefore, interrupting commands need a way to reset the available systems of commands they interrupt. Remember to do this future me 
		
		unsigned char requirementFlags;
		std::function<bool()> backingFunction;

	protected:
		//virtual void ThisIsAbstract() = 0;

		Scheduleable(std::function<bool()> backingFunction, unsigned char requirementFlags);

		//Scheduleable(std::function<bool()> backingFunction);

		Scheduleable();

	public:
		bool IsDead;

		~Scheduleable();

		void AddRequirement(unsigned char newRequirementFlags);

		bool RunIfReady(unsigned char availableSystem);
	};

private:
	std::unordered_map<int, std::shared_ptr<Scheduleable>> database;
	int nextAvailableID;

public:

	FunctionManager();

	int AddToDatabase(Scheduleable* scheduledItem);

	bool RunIfReady(int scheduledID, unsigned char availableSystem); //Runs the IScheduleable with the specified ID if the system requirements are met

	void Remove(int ID);
};

