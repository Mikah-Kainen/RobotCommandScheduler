#pragma once
#include "base_groups/reschedulable_group.h"

class ParallelGroup : public ReschedulableGroup
{
private:


protected:


public:

	//ParallelGroup(std::vector<Schedulable> schedulableList, unsigned char requirements);

	ParallelGroup(std::vector<std::shared_ptr<Schedulable>> schedulablesToSchedule);
};
