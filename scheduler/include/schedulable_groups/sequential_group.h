#pragma once
#include "base_groups/reschedulable_group.h"

class SequentialGroup : public ReschedulableGroup
{
private:

protected:

public:
	SequentialGroup(std::vector<std::shared_ptr<Schedulable>> schedulablesToSchedule);

	//SequentialGroup(Schedulable** schedulablesToSchedule, int numberOfSchedulables);
};

