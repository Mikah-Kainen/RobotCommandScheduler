#pragma once

#include <deque>
#include <vector>
#include <list>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "../schedulable.h"
#include "../static.h"


//GroupBase uses PackedIDs to keep track of important flags, Database uses UnpackedIDs to abstract the flags away and for ease of debugging
class GroupBase : public Schedulable
{
protected:
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
	static const unsigned int ShouldInitializeMask = 1 << ShouldInitializeDigit;
	//static const unsigned int InterruptableMask = 1 << InterruptableDigit;

	static const unsigned int UnpackMask = ~(BarMask | ShouldInitializeMask | EndEarlyMask);
	//Cool Flags: EndEarly, Bar, Interruptable, Initialized, RunDefault?, Branching???

	static bool IsFlagSet(unsigned int packedID, unsigned int mask);

	static unsigned char GetRequirementFlags(std::vector<std::shared_ptr<Schedulable>> schedulables);

	enum class GroupTypes
	{
		Base,
		Parallel,
		Sequential,
		Loop,
		Conditional,
		TryCatch,
		WhenAny,
		FSM,
	};

	enum class GroupFlags : unsigned char
	{
		None = 0x0,
		EndEarly = 0x1,
	};

	static unsigned int Unpack(unsigned int packedID);

	static unsigned int Pack(unsigned int  unpackedID);

#pragma region ProtectedFlagHelperFunctions
	static bool IsSchedulableBarSet(unsigned int packedID);

	static bool IsSchedulableShouldInitializeSet(unsigned int packedID);

	static bool IsSchedulableEndEarlySet(unsigned int packedID);
#pragma endregion

protected:
	//Interesting link: https://jguegant.github.io/blogs/tech/performing-try-emplace.html
	class DictionaryManager //Rename PackedID to FlagsID and UnpackedID to DictionaryID //maybe just have dictionaryFunctions unpack
	{
	private:
		unsigned int nextAvailableID; //Starts at 1 to allow 0 for NoID checks (such as with the Schedulable::AuxiliaryID)
		std::function<unsigned int(unsigned int)> removeFlagsFunc;

	public:
		std::unordered_map<unsigned int, std::shared_ptr<Schedulable>> schedulableMap;
		std::unordered_map<unsigned int, std::vector<std::function<void(GroupBase&)>>> endBehaviors;

		DictionaryManager(std::function<unsigned int(unsigned int)> removeFlagsFunc);

		DictionaryManager();

		DictionaryManager(const DictionaryManager& copyDictionaryManager);

		~DictionaryManager();

		unsigned int Add(std::shared_ptr<Schedulable> scheduledItem);

		bool RunIfReady(unsigned int scheduledID, unsigned char availableSystem); //Runs the ISchedulable with the specified ID if the system requirements are met

		void Remove(unsigned int ID);

		void ResetAvailability(unsigned int ID);

		void SubscribeToEnd(unsigned int targetID, std::function<void(GroupBase&)> endBehavior);
	};

	enum class Behaviors
	{
		Bar,
		EndEarly,
		Initialize,
		Run,
	};

	Behaviors GetNextBehavior(int packedID);

protected:
	GroupTypes groupType;
	unsigned int groupID;
	unsigned char groupFlags;

	//virtual void InitializeGroup() = 0;

	virtual void Remove(unsigned int packedID) = 0;

	virtual bool Return(bool isFinished) = 0;
	//virtual bool PostRun(std::vector<unsigned int> packedIDsToDelete) = 0;

	std::unordered_set<unsigned int> requirementFreeSchedulables;

	std::vector<unsigned int>* schedule; //maybe change this to a list of nodes of the current schedulableID
	unsigned int* currentIndices;
	DictionaryManager database;

	std::vector<std::function<void(GroupBase&)>> initializeFunctions; //used to set CleanupFunctions in the CopyConstructor
	//bool shouldInitializeOrHasRestarted = true;

	GroupBase(unsigned char systemFlags, GroupTypes type);

	GroupBase(std::vector<unsigned char> systemFlags, GroupTypes type);

	GroupBase(std::vector<Systems> schedulerSystems, GroupTypes type);


	unsigned int Schedule(std::shared_ptr<Schedulable> schedulable);

	unsigned int Schedule(std::function<bool()> function, unsigned char requirementFlags);

	unsigned int Schedule(std::function<bool()> function, std::vector<Systems> requiredSystems);

	//If I ever need these I can have a special group that exposes their functionality; however, it is probably unnecessary to make them public in every group

	//void SetShouldInitialize(unsigned int packedID);

	//void SetEndEarly(unsigned int packedID);

	void ReplaceIDUnpacked(unsigned int unpackedID, unsigned int newPackedID);

	//virtual void Cleanup(int packedID) = 0; //ADD THIS! Then add derived class NonConsumingGroup and ConsumingGroup

	//void AddToInitialize(std::function<void(GroupBase&)> initializeFunction);

	bool IsGroupEndEarlySet();

public:
	GroupBase(const GroupBase& copyGroupBase);

#pragma region PublicFlagHelperFunctions
	void AddBar(unsigned int packedID); //multiple bars can be set

	void RemoveBar(unsigned int packedID);

	void SetSchedulableEndEarly(unsigned int packedID, bool value); //Sets the EndEarly bit for a particular ID to value

	void SetSchedulableShouldInitialize(unsigned int packedID, bool value); //Sets the EndEarly bit for a particular ID to value

	void SetGroupEndEarly(bool value); //Sets the EndEarly flag for the whole group to value
#pragma endregion

	void ReplaceID(unsigned int oldPackedID, unsigned int newPackedID);

	void SubscribeToEnd(unsigned int targetID, std::function<void(GroupBase&)> endBehavior);
	
	void Update();

	bool Run() override; //This function does too much. I should split it at some point

	virtual void Initialize() override;
};

