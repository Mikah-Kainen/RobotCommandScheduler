//#include "../../inc/SchedulerInc/SequentialGroup.h"
#include "SequentialGroup.h"


SequentialGroup::SequentialGroup(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleablesToSchedule)
	:GroupBase(GetRequirementFlags(scheduleablesToSchedule), SchedulerTypes::Sequential)
{
	unsigned int previousID = 1 << 31;
	for (std::shared_ptr<FunctionManager::Scheduleable> scheduleable : scheduleablesToSchedule)
	{
		scheduleable->AddRequirement(requirementFlags); //this system is a bit broken
		
		unsigned int currentID = GroupBase::Schedule(scheduleable);
		//if (previousID != 1 << 31) //figure out a better way to do this check (probably just switch to for loop starting at index 1
		//{
		//	GroupBase::SetBar(currentID);
		//}
		//currentID = GroupBase::Schedule(scheduleable);
		//AddBar(currentID); //AddBar << 1 + 1 to a Bar flag maintained by the scheduler for each ID (Bar flag can be the first digit or two of the ID given by the functionManager)
		//functionManager.Subscribe(previousID(target), [&](){RemoveBar(currentID);}(func to run when target is done))
		//Subscribe returns true if the targetID exists
		//If a function tries to run but its bar flag is not zero, run the default behaviour
		//Add max bar flag and min bar flag to static(7, 1)
		previousID = currentID;
	}
}

//SequentialGroup::SequentialGroup(FunctionManager::Scheduleable** scheduleablesToSchedule, int numberOfScheduleables)
//	:SequentialGroup(std::vector<FunctionManager::Scheduleable*>(numberOfScheduleables, scheduleablesToSchedule[0]))
//{
//}