#pragma once
#include "SchedulerBase.h"

class SequentialGroup : public SchedulerBase
{
private:

protected:

public:
	SequentialGroup(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleablesToSchedule);

	//SequentialGroup(FunctionManager::Scheduleable** scheduleablesToSchedule, int numberOfScheduleables);
};

