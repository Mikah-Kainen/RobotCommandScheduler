#pragma once
#include "SchedulerBase.h"

class SequentialGroup : public SchedulerBase
{
private:

protected:

public:
	SequentialGroup(std::vector<FunctionManager::Scheduleable*> scheduleablesToSchedule);

	//SequentialGroup(FunctionManager::Scheduleable** scheduleablesToSchedule, int numberOfScheduleables);
};
