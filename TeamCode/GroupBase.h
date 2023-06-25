#pragma once
#include <deque>
#include <vector>
#include <list>
#include <iostream>
#include <unordered_map>
#include <memory>
#include "Scheduleable.h"
#include "Static.h"


//GroupBase uses PackedIDs to keep track of important flags, Database uses UnpackedIDs to abstract the flags away and for ease of debugging
class GroupBase : public Scheduleable
{
protected:
	static unsigned char GetRequirementFlags(std::vector<std::shared_ptr<Scheduleable>> scheduleables);

	enum class SchedulerTypes
	{
		Base,
		Parallel,
		Sequential,
		Loop,
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

protected:
	//Interesting link: https://jguegant.github.io/blogs/tech/performing-try-emplace.html
	class Database
	{
	private:
		unsigned int nextAvailableID;

	public:
		std::unordered_map<unsigned int, std::shared_ptr<Scheduleable>> scheduleableMap;
		std::unordered_map<unsigned int, std::vector<std::function<void(GroupBase&)>>> endBehaviors;

		Database();

		Database(const Database& copyDatabase);

		~Database();

		unsigned int Add(std::shared_ptr<Scheduleable> scheduledItem);

		bool RunIfReady(unsigned int scheduledID, unsigned char availableSystem); //Runs the IScheduleable with the specified ID if the system requirements are met

		void Remove(unsigned int ID);

		void ResetAvailability(unsigned int ID);

		void SubscribeToEnd(unsigned int targetID, std::function<void(GroupBase&)> endBehavior);
	};

protected:
	SchedulerTypes schedulerType;
	unsigned int schedulerID; //for debugging

	virtual void Initialize() = 0;

	virtual void Remove(unsigned int packedID) = 0;

	virtual bool Return(bool isFinished) = 0;
	//virtual bool PostRun(std::vector<unsigned int> packedIDsToDelete) = 0;

	std::vector<unsigned int>* schedule;
	unsigned int* currentIndices;
	Database database;

	std::vector<std::function<void(GroupBase&)>> initializeFunctions; //used to set CleanupFunctions in the CopyConstructor
	//bool shouldInitializeOrHasRestarted = true;

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

	//virtual void Cleanup(int packedID) = 0; //ADD THIS! Then add derived class NonConsumingGroup and ConsumingGroup

	//void AddToInitialize(std::function<void(GroupBase&)> initializeFunction);

public:
	GroupBase(const GroupBase& copyGroupBase);

	void SetBar(unsigned int packedID); //multiple bars can be set

	void RemoveBar(unsigned int packedID);

	void ReplaceID(unsigned int oldPackedID, unsigned int newPackedID);

	void SubscribeToEnd(unsigned int targetID, std::function<void(GroupBase&)> endBehavior);
	
	void Update();

	bool Run() override;
};

