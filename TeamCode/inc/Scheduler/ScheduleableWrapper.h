#pragma once
#include "Scheduleable.h"

class ScheduleableWrapper
{
private:
	std::shared_ptr<Scheduleable> wrappedScheduleable;

public:

	ScheduleableWrapper(std::shared_ptr<Scheduleable> scheduleableToWrap);

	ScheduleableWrapper(const ScheduleableWrapper& copyScheduleableWrapper);

	//NOTE to self: Update CommandBuilder to be more like this
	ScheduleableWrapper& SetInitializationScheduleable(std::shared_ptr<Scheduleable> initializationScheduleable);

	ScheduleableWrapper& SetInitialization(std::function<bool()> initializeBehavior); //Creates a new initialization Scheduleable with the same requirements as the core Scheduleable

	ScheduleableWrapper& AddLoop(std::function<bool()> loopCondition);

	ScheduleableWrapper& AddLoop(int loopCount);

	ScheduleableWrapper& AddCondition(bool condition);

	std::shared_ptr<Scheduleable> GetScheduleable();

};

