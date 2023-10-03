#pragma once
#include <functional>
#include <memory>
#include "Static.h"

class Scheduleable
{
private:
	enum class ScheduleableStates
	{
		Initialize,
		Run,
		Cleanup,
		Return,
	};

	static ScheduleableStates GetNextState(ScheduleableStates state);

	unsigned char availableSystems;
	std::function<bool()> backingFunction; //Change to virtual function
	std::shared_ptr<Scheduleable> initializationScheduleable;
	std::shared_ptr<Scheduleable> cleanupScheduleable;
	ScheduleableStates startingState;
	ScheduleableStates endingState;
	bool lock;
	unsigned int unlockKey; //unlockKey = 16bits SchedulerID | 16bits ScheduleableID
	//UnpackedID, Can safely start at zero because SchedulerID starts at 1
	//Alsooooo, this might need to be special cased when I use typedef for the IDs... (I'm probably gonna forget this so sorry future self for the debugging pain)

	virtual bool Run();

protected:
	Scheduleable(unsigned char requirementFlags);

	void SetInitializationScheduleableDirect(std::shared_ptr<Scheduleable> scheduleable);

	void SetCleanupScheduleableDirect(std::shared_ptr<Scheduleable> scheduleable);

public:
	//virtual void ThisIsAbstract() = 0;
	ScheduleableStates currentState;
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

	void AddAvailability(unsigned char availableSystem);

	bool IsReady();

	bool RunFSM();

	//virtual bool Initialize(); //Use this and just keep state of initialization of InitializeFunc, MainRun, and CleanupFunc if I need initializations with more than one cycle
	virtual void Initialize();

	//void InitializeFSM();

	void ResetAvailability();

	bool SetInitializationScheduleable(std::shared_ptr<Scheduleable> scheduleable);

	bool SetCleanupScheduleable(std::shared_ptr<Scheduleable> scheduleable);

	bool TryUnlock(unsigned int key);

	void ResetLock();
};