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

	//Bar flags use one bit per value i.e. 3 bits = 3 values as opposed to 3 bits = 8 values. This is for readability so that anything with a bar set will be an odd number
	//BarBits: 00I for 1 bar, 0II for two bars, III for three bars. To check for bar just check first bit, to add bar shift left, to remove bar shift right
	static const int BarBitCount = 3; //Represents the number of Bar flags
	static const unsigned int BarMask = UINT32_MAX >> (32 - BarBitCount);
	static const unsigned int BarMin = 1;

	//The digits that represent the various flags (0-31)
	static const unsigned int EndEarlyDigit = 31; //IF Val ~ 4mil
	static const unsigned int ShouldInitializeDigit = 30; //IF Val ~ 2mil
	//static const unsigned int InterruptableDigit = 29;  //IF Val ~ 1mil

	static const unsigned int EndEarlyMask = 1 << EndEarlyDigit;
	//static const unsigned int ShouldInitializeMask = 1 << ShouldInitializeDigit;
	//static const unsigned int InterruptableMask = 1 << InterruptableDigit;

	static const unsigned int UnpackMask = ~(BarMask | /*ShouldInitializeMask |*/ EndEarlyMask);
	//Cool Flags: EndEarly, Bar, Interruptable, Initialized, RunDefault?, Branching???

	static unsigned int Unpack(unsigned int packedID);

	static unsigned int Pack(unsigned int  unpackedID);

	//static bool IsBarSet(unsigned int packedID);

	static bool IsFlagSet(unsigned int packedID, unsigned int mask);

	//static bool IsShouldInitializeSet(unsigned int packedID);

	//static bool IsEndEarlySet(unsigned int packedID);

private:
	std::vector<unsigned int>* schedule;
	unsigned int* currentIndices;
	FunctionManager functionManager;
	SchedulerTypes schedulerType;
	unsigned int schedulerID; //for debugging
	bool shouldInitializeOrHasRestarted = true;

protected:
	std::vector<std::function<void(GroupBase&)>> initializeFunctions; //used to set CleanupFunctions in the CopyConstructor

	GroupBase(unsigned char systemFlags, SchedulerTypes type);

	GroupBase(std::vector<unsigned char> systemFlags, SchedulerTypes type);

	GroupBase(std::vector<Systems> schedulerSystems, SchedulerTypes type);


	unsigned int Schedule(std::shared_ptr<Scheduleable> scheduleable);

	unsigned int Schedule(std::function<bool()> function, unsigned char requirementFlags);

	unsigned int Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems);

	//If I ever need these I can have a special group that exposes their functionality; however, it is probably unnecessary to make them public in every group

	//void SetShouldInitialize(unsigned int packedID);

	//void SetEndEarly(unsigned int packedID);

	void ReplaceIDUnpacked(unsigned int unpackedID, unsigned int newPackedID);

	//void AddToInitialize(std::function<void(GroupBase&)> initializeFunction);

public:
	GroupBase(const GroupBase& copyGroupBase);

	void SetBar(unsigned int packedID); //multiple bars can be set

	void RemoveBar(unsigned int packedID);

	void ReplaceID(unsigned int oldPackedID, unsigned int newPackedID);

	void SubscribeToEnd(unsigned int targetID, std::function<void()> endBehavior);
	
	void Update();

	bool Run() override;
};

