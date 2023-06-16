#pragma once
#include <deque>
#include <vector>
#include <list>
#include <iostream>
#include "FunctionManager.h"
#include "Static.h"


class GroupBase : public FunctionManager::Scheduleable
{
protected:
	static unsigned char GetRequirementFlags(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleables);

	enum class SchedulerTypes
	{
		Base,
		Parallel,
		Sequential,
	};

	//These flags use one bit per value i.e. 3 bits = 3 values as opposed to 3 bits = 8 values. This is for readability so that anything with a bar set will be an odd number
	//Bar flags are the least significant bits in a number and EndEarly flags are the most significant bits. This way telling if these flags are simple: if a number is big, EndEarly flag is set; if a number is odd, Bar flag is set
	static const int BarBitCount = 3; //Represents the number of Bar flags
	static const int EndEarlyBitCount = 1; //Represents the number of EndEarly flags(not sure why you'd want more than 1)

	//BarBits: 00I for 1 bar, 0II for two bars, III for three bars. To check for bar just check first bit, to add bar shift left, to remove bar shift right
	static const unsigned int BarMask = UINT32_MAX >> (32 - BarBitCount);
	static const unsigned int BarMin = 1;
	static const unsigned int EndEarlyMask = UINT32_MAX << (32 - EndEarlyBitCount);
	static const unsigned int EndEarlyMin = 1 << (32 - EndEarlyBitCount);
	static const unsigned int UnpackMask = ~(BarMask | EndEarlyMask);

	static unsigned int Unpack(unsigned int packedID);

	static unsigned int Pack(unsigned int  unpackedID);

	static bool IsBarSet(unsigned int packedID);

	static bool IsEndEarlySet(unsigned int packedID);

private:

	//std::unordered_map<Systems, std::list<unsigned int>> schedule;
	std::vector<unsigned int>* schedule;
	FunctionManager functionManager;
	SchedulerTypes schedulerType;
	unsigned char currentlyRunningSystems;
	unsigned int schedulerID;

protected:

	GroupBase(unsigned char systemFlags, SchedulerTypes type);

	GroupBase(std::vector<unsigned char> systemFlags, SchedulerTypes type);

	GroupBase(std::vector<Systems> schedulerSystems, SchedulerTypes type);


	int Schedule(std::shared_ptr<Scheduleable> scheduleable);

	int Schedule(std::function<bool()> function, unsigned char requirementFlags);

	int Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems);

	//If I ever need these I can have a special group that exposes their functionality; however, it is probably unnecessary to make them public in every group
	void SetBar(unsigned int packedID); //multiple bars can be set

	void RemoveBar(unsigned int packedID);

	void SetEndEarly(unsigned int packedID);

	void Subscribe(unsigned int targetID, std::function<void()> endBehavior);

public:
	GroupBase(const GroupBase& copyGroupBase);
	
	void Update();

	bool Run() override;
};

