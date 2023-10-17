#pragma once
#include "RescheduleableGroup.h"

//Ends when any scheduleable in the group ends. 
//Does not guarentee that only one scheduleable will finish before ending, because multiple scheduleables can finish in the same cycle
//Since ConditionWrappers return true to finish when their condition is not met, WhenAnyGroups will finish as soon as any internal ConditionWrapper's condition is not met
class WhenAnyGroup : public RescheduleableGroup
{
private:

protected:
	
public:
	WhenAnyGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule);
};

//WhenAnyGroup.cpp modelled after SequentialGroup.cpp

/*
Tested ConditionWrapper more thoroughly.
Added RunConditionally function to make wrapping scheduleables with conditions more convenient.
Started implementing WhenAnyGroup.
*/