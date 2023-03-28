#pragma once
#include <memory>
#include <list>
#include <unordered_map>
#include <exception>


/*
make Manager specific to SchedulerFunctions
Scheduler needs a way to check if all its functions are done(maybe a for loop & the count and checking if 0 at the end), so command groups can be converted to a scheduler which can be converted to a function
Manager should have a subscribe function, which subscribes an ID to the end function of a different ID(ScheduledCommand should have a list of functions to run when it ends)
ScheduledFunction should be in Manager, not in Scheduler
*/

template <typename T>
class Manager
{
public:

	Manager()
		: nextAvailableID{ 0 }, backingMap{ std::unordered_map<int, std::unique_ptr<T>>() } {}

	std::unique_ptr<T>& Get(int ID)
	{
		return backingMap[ID];
	}

	void Release(int ID)
	{
		backingMap.erase(ID);
	}

	int Add(T& value)
	{
		int newID = nextAvailableID++;
		backingMap.insert(newID, std::make_unique<T>(value));
		return newID;
	}

private:
	int nextAvailableID;
	std::unordered_map<int, std::unique_ptr<T>> backingMap;

};

//template <typename T>
//class Manager
//{
//public:
//
//	Manager(int maxSize)
//		: maxSize{ maxSize }, memory{new std::unique_ptr<T>[maxSize]}
//	{
//		for (int i = 0; i < maxSize; i ++)
//		{
//			availableIDs.push_back(i);
//		}
//	}
//
//	std::unique_ptr<T>& Get(int ID)
//	{
//		return memory[ID]&;
//	}
//
//	void Release(int ID)
//	{
//		availableIDs.push_back(ID);
//		memory[ID] = nullptr;
//	}
//
//	int Add(T& value) //make sure that T still exists in the Manager even if it goes out of scope in the place that called the manager
//	{
//		if (availableIDs.size == 0)
//		{
//			throw std::exception("Manager out of memory");
//		}
//		int newID = availableIDs.front();
//		availableIDs.pop_front();
//		memory[newID] = std::make_unique<T>(value);
//		return newID;
//	}
//
//private:
//	int maxSize; //use to find the optimal size of the manager
//	std::unique_ptr<T>* memory;
//	std::list<int> availableIDs;
//};
