#pragma once
#include "GroupBase.h"

class SequentialGroup : public GroupBase
{
private:

protected:

public:
	SequentialGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule);

	//SequentialGroup(Scheduleable** scheduleablesToSchedule, int numberOfScheduleables);
};

