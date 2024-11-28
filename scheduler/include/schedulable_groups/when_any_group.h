#pragma once
#include "base_groups/reschedulable_group.h"

//Ends when any schedulable in the group ends. 
//Does not guarentee that only one schedulable will finish before ending, because multiple schedulables can finish in the same cycle
//Since ConditionSchedulables return true to finish when their condition is not met, WhenAnyGroups will finish as soon as any internal ConditionSchedulable's condition is not met
class WhenAnyGroup : public ReschedulableGroup
{
private:

protected:
	
public:
	WhenAnyGroup(std::vector<std::shared_ptr<Schedulable>> schedulablesToSchedule);
};

//WhenAnyGroup.cpp modelled after SequentialGroup.cpp

/*
Tested ConditionSchedulable more thoroughly.
Added RunConditionally function to make wrapping schedulables with conditions more convenient.
Started implementing WhenAnyGroup.
*/