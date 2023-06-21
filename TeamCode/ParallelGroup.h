#pragma once
#include "RescheduleableGroup.h"

class ParallelGroup : public RescheduleableGroup
{
private:


protected:


public:

	//ParallelGroup(std::vector<Scheduleable> scheduleableList, unsigned char requirements);

	ParallelGroup(std::vector<std::shared_ptr<Scheduleable>> scheduleablesToSchedule);
};
