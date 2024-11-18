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

	virtual bool Run();

protected:
	/*Ok this is probably not actually necessary. The group knows IDs when scheduling, and initialization and cleanup functions can be added to the lists that take care of the rest
	enum class ReturnCodes : unsigned int
	{
		Unassigned = 0,
		None = 1,
		ReInitialize = 2, //Eventually Change LoopGroup to use this
		SwapMapping = 256, //Uses auxiliary field to know which mapping to swap to
						   //Make sure to reset lock!
		//RunNextScheduleable, //HAHAHAAHAHAH I BETTER NOT USE THIS BECAUSE IT WILL KILLLLLLLLLLLLLLLLL EVERYTHING
	};
	ReturnCodes returnCode;
	unsigned int auxiliarlyUnpackedID; //used to supplement the certain ReturnCodes behavior, such as SwapMapping
	*/

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
};

//std::shared_ptr<ConditionWrapper> ConditionWrapper::RunConditionally(std::shared_ptr<Scheduleable> scheduleable, std::function<bool()> condition)
//{
//	return std::make_shared<ConditionWrapper>(ConditionWrapper(scheduleable, condition));
//}