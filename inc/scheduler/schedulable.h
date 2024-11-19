#pragma once
#include <functional>
#include <memory>
#include "static.h"

class Schedulable
{
private:
	enum class SchedulableStates
	{
		Initialize,
		Run,
		Cleanup,
		Return,
	};

	static SchedulableStates GetNextState(SchedulableStates state);

	unsigned char availableSystems;
	std::function<bool()> backingFunction; //Change to virtual function
	std::shared_ptr<Schedulable> initializationSchedulable;
	std::shared_ptr<Schedulable> cleanupSchedulable;
	SchedulableStates startingState;
	SchedulableStates endingState;

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
		//RunNextSchedulable, //HAHAHAAHAHAH I BETTER NOT USE THIS BECAUSE IT WILL KILLLLLLLLLLLLLLLLL EVERYTHING
	};
	ReturnCodes returnCode;
	unsigned int auxiliarlyUnpackedID; //used to supplement the certain ReturnCodes behavior, such as SwapMapping
	*/

	Schedulable(unsigned char requirementFlags);

	void SetInitializationSchedulableDirect(std::shared_ptr<Schedulable> schedulable);

	void SetCleanupSchedulableDirect(std::shared_ptr<Schedulable> schedulable);

public:
	//virtual void ThisIsAbstract() = 0;
	SchedulableStates currentState;
	bool IsDead;
	unsigned char requirementFlags;

	Schedulable(std::function<bool()> backingFunction, unsigned char requirementFlags);

	Schedulable(std::function<bool()> backingFunction, Systems requiredSystem);

	//Schedulable(std::function<bool()> backingFunctionCopy, unsigned char requirementFlags, bool passingByCopy);

	//Schedulable(std::function<bool()> backingFunctionCopy, Systems requiredSystem, bool passingByCopy);

	//Schedulable();

	Schedulable(const Schedulable& copyScheduelable);

	~Schedulable();

	unsigned char GetRequirementFlags();

	void AddRequirement(unsigned char newRequirementFlags);

	void AddAvailability(unsigned char availableSystem);

	bool IsReady();

	bool RunFSM();

	//virtual bool Initialize(); //Use this and just keep state of initialization of InitializeFunc, MainRun, and CleanupFunc if I need initializations with more than one cycle
	virtual void Initialize();

	//void InitializeFSM();

	void ResetAvailability();

	bool SetInitializationSchedulable(std::shared_ptr<Schedulable> schedulable);

	bool SetCleanupSchedulable(std::shared_ptr<Schedulable> schedulable);
};

//std::shared_ptr<ConditionWrapper> ConditionWrapper::RunConditionally(std::shared_ptr<Schedulable> schedulable, std::function<bool()> condition)
//{
//	return std::make_shared<ConditionWrapper>(ConditionWrapper(schedulable, condition));
//}