#pragma once
#include "GroupBase.h"

class SequentialGroup : public GroupBase
{
private:

protected:

public:
	SequentialGroup(std::vector<std::shared_ptr<FunctionManager::Scheduleable>> scheduleablesToSchedule);

	//SequentialGroup(FunctionManager::Scheduleable** scheduleablesToSchedule, int numberOfScheduleables);
};

