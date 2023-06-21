#pragma once
#include "RescheduleableGroup.h"

class SequentialGroup : public RescheduleableGroup
{
private:

protected:

public:
	SequentialGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule);

	//SequentialGroup(Scheduleable** scheduleablesToSchedule, int numberOfScheduleables);
};

